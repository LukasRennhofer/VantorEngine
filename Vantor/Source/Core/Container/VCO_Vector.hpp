// VCO_Vector.hpp - Custom Vector implementation for the Vantor engine
// This is a simplified std::Vector-like container with basic dynamic array functionality
// Provides efficient memory management with allocator support

#pragma once

#include <cassert>
#include <cstdint>
#include <memory>
#include <utility>
#include <initializer_list>
#include <new>

namespace Vantor::Core
{
	// Custom Vector container template class
	// T: element type, A: allocator type (defaults to std::allocator<T>)
	template<typename T, typename A = std::allocator<T>>
	class Vector
	{
	public:
		using value_type = T;

		// Default constructor - creates empty Vector or with specified initial size
		inline Vector(size_t initial_size = 0)
		{
			resize(initial_size);
		}

		// Copy constructor - creates deep copy of another Vector
		inline Vector(const Vector<T>& source_Vector)
		{
			copy_from(source_Vector);
		}

		// Move constructor - transfers ownership from another Vector
		inline Vector(Vector<T>&& source_Vector) noexcept
		{
			move_from(std::move(source_Vector));
		}

		// Initializer list constructor - creates Vector from brace-enclosed list
		inline Vector(std::initializer_list<T> init_list)
		{
			reserve(init_list.size());
			for (auto& element : init_list)
			{
				push_back(element);
			}
		}

		// Destructor - cleans up all elements and deallocates memory
		inline ~Vector()
		{
			clear();
			if (m_data_ptr != nullptr)
			{
				m_allocator.deallocate(m_data_ptr, m_capacity);
			}
		}

		// Copy assignment - replaces contents with deep copy of another Vector
		inline Vector<T>& operator=(const Vector<T>& source_Vector)
		{
			copy_from(source_Vector);
			return *this;
		}

		// Move assignment - replaces contents by transferring ownership
		inline Vector<T>& operator=(Vector<T>&& source_Vector)
		{
			move_from(std::move(source_Vector));
			return *this;
		}

		// Array subscript operator - provides direct access to element at index (no bounds checking)
		constexpr T& operator[](size_t element_index) noexcept
		{
			assert(m_current_size > 0 && element_index < m_current_size);
			return m_data_ptr[element_index];
		}

		// Const array subscript operator - provides read-only access to element at index
		constexpr const T& operator[](size_t element_index) const noexcept
		{
			assert(m_current_size > 0 && element_index < m_current_size);
			return m_data_ptr[element_index];
		}

		// Reserve memory for at least the specified number of elements (doesn't change size)
		inline void reserve(size_t requested_capacity)
		{
			if (requested_capacity > m_capacity)
			{
				size_t old_capacity = m_capacity;
				T* old_data_ptr = allocate_and_move(requested_capacity);
				if (old_data_ptr != nullptr)
				{
					m_allocator.deallocate(old_data_ptr, old_capacity);
				}
			}
		}

		// Resize Vector to contain exactly the specified number of elements
		inline void resize(size_t new_size)
		{
			if (new_size > m_current_size)
			{
				reserve(new_size);

				// Expand the Vector - default-construct new elements
				for (size_t i = m_current_size; i < new_size; ++i)
				{
					new (m_data_ptr + i) T();
				}
				m_current_size = new_size;
			}
			else if (new_size < m_current_size)
			{
				// Shrink the Vector - destroy excess elements
				while (m_current_size > new_size)
				{
					pop_back();
				}
			}
		}

		// Reduce capacity to match current size (frees unused memory)
		inline void shrink_to_fit()
		{
			if (m_capacity > m_current_size)
			{
				size_t old_capacity = m_capacity;
				T* old_data_ptr = allocate_and_move(m_current_size);
				if (old_data_ptr != nullptr)
				{
					m_allocator.deallocate(old_data_ptr, old_capacity);
				}
			}
		}

		// Construct element in-place at the end of the Vector using provided arguments
		template<typename... ConstructorArgs>
		inline T& emplace_back(ConstructorArgs&&... constructor_args)
		{
			size_t old_capacity = m_capacity;
			T* old_data_ptr = nullptr;
			const size_t required_capacity = m_current_size + 1;

			// Check if we need to grow the capacity
			if (required_capacity > m_capacity)
			{
				// Double the capacity to amortize future allocations
				old_data_ptr = allocate_and_move(required_capacity * 2);
			}

			// Construct new element at the end
			T* new_element_ptr = new (m_data_ptr + m_current_size) T(std::forward<ConstructorArgs>(constructor_args)...);
			m_current_size++;

			// Clean up old allocation if we reallocated
			if (old_data_ptr != nullptr)
			{
				m_allocator.deallocate(old_data_ptr, old_capacity);
			}
			return *new_element_ptr;
		}

		// Add element to the end of the Vector (copy version)
		inline void push_back(const T& element)
		{
			emplace_back(element);
		}

		// Add element to the end of the Vector (move version)
		inline void push_back(T&& element)
		{
			emplace_back(std::move(element));
		}

		// Remove all elements from the Vector (calls destructors but doesn't deallocate memory)
		constexpr void clear() noexcept
		{
			for (size_t i = 0; i < m_current_size; ++i)
			{
				m_data_ptr[i].~T();
			}
			m_current_size = 0;
		}

		// Remove the last element from the Vector
		constexpr void pop_back() noexcept
		{
			assert(m_current_size > 0);
			m_data_ptr[--m_current_size].~T();
		}
		
		// Remove element at specified position (shifts remaining elements left)
		constexpr void erase(T* position) noexcept
		{
			assert(position >= begin());
			assert(position < end());
			std::move(position + 1, end(), position); // Shift elements left to fill gap
			pop_back();                               // Destroy the now-duplicate last element
		}

		// Get reference to the last element
		constexpr T& back() noexcept
		{
			assert(m_current_size > 0);
			return m_data_ptr[m_current_size - 1];
		}

		// Get const reference to the last element
		constexpr const T& back() const noexcept
		{
			assert(m_current_size > 0);
			return m_data_ptr[m_current_size - 1];
		}

		// Get reference to the first element
		constexpr T& front() noexcept
		{
			assert(m_current_size > 0);
			return m_data_ptr[0];
		}

		// Get const reference to the first element
		constexpr const T& front() const noexcept
		{
			assert(m_current_size > 0);
			return m_data_ptr[0];
		}

		// Get direct pointer to underlying array
		constexpr T* data() noexcept
		{
			return m_data_ptr;
		}

		// Get const pointer to underlying array
		constexpr const T* data() const noexcept
		{
			return m_data_ptr;
		}

		// Bounds-checked element access (same as operator[] but with explicit assertion)
		constexpr T& at(size_t element_index) noexcept
		{
			assert(m_current_size > 0 && element_index < m_current_size);
			return m_data_ptr[element_index];
		}

		// Const bounds-checked element access
		constexpr const T& at(size_t element_index) const noexcept
		{
			assert(m_current_size > 0 && element_index < m_current_size);
			return m_data_ptr[element_index];
		}

		// Get the number of elements currently in the Vector
		constexpr size_t size() const noexcept
		{
			return m_current_size;
		}

		// Check if the Vector contains no elements
		constexpr bool empty() const noexcept
		{
			return m_current_size == 0;
		}

		// Get iterator to first element
		constexpr T* begin() noexcept
		{
			return m_data_ptr;
		}

		// Get const iterator to first element
		constexpr const T* begin() const noexcept
		{
			return m_data_ptr;
		}

		// Get iterator to one past the last element
		constexpr T* end() noexcept
		{
			return m_data_ptr + m_current_size;
		}

		// Get const iterator to one past the last element
		constexpr const T* end() const noexcept
		{
			return m_data_ptr + m_current_size;
		}

	private:
		// Allocate new memory and move existing elements to it
		// Returns pointer to old data if reallocation occurred, nullptr otherwise
		inline T* allocate_and_move(size_t new_capacity)
		{
			if (new_capacity > m_current_size)
			{
				m_capacity = new_capacity;

				// Allocate new memory block
				T* new_allocation = m_allocator.allocate(m_capacity);

				// Move existing elements to new location
				for (size_t i = 0; i < m_current_size; ++i)
				{
					new (new_allocation + i) T(std::move(m_data_ptr[i]));
				}

				// Swap data pointers and return old allocation for cleanup
				std::swap(m_data_ptr, new_allocation);
				return new_allocation;
			}
			return nullptr;
		}

		// Copy all elements from another Vector (used by copy constructor and assignment)
		inline void copy_from(const Vector<T>& source_Vector)
		{
			resize(source_Vector.size());
			for (size_t i = 0; i < m_current_size; ++i)
			{
				m_data_ptr[i] = source_Vector.m_data_ptr[i];
			}
		}

		// Transfer ownership from another Vector (used by move constructor and assignment)
		inline void move_from(Vector<T>&& source_Vector)
		{
			clear();
			if (m_data_ptr != nullptr)
			{
				m_allocator.deallocate(m_data_ptr, m_capacity);
			}

			// Take ownership of source Vector's data
			m_capacity = source_Vector.m_capacity;
			m_current_size = source_Vector.m_current_size;
			m_data_ptr = source_Vector.m_data_ptr;

			// Leave source Vector in valid empty state
			source_Vector.m_data_ptr = nullptr;
			source_Vector.m_current_size = 0;
			source_Vector.m_capacity = 0;
		}

		// Members
		T* m_data_ptr = nullptr;
		size_t m_current_size = 0;
		size_t m_capacity = 0;
		A m_allocator;
	};
}