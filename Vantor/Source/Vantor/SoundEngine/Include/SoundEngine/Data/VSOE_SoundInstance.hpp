/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#pragma once

#include <Math/Linear/VMA_Vector.hpp>

#include <cstdint>

namespace SoLoud { class Soloud; } // Soloud Forward Decl.

namespace VE::Internal::SoundEngine {
    using VSoundHandle = unsigned int;

    class VSoundInstance {
        public:
            VSoundInstance(
                SoLoud::Soloud& p_backend,
                VSoundHandle p_handle,
                bool p_spatial
            );

            enum class EAttenuationModel : uint8_t
            {
                NONE,
                INVERSE_DISTANCE,
                LINEAR_DISTANCE,
                EXPONENTIAL_DISTANCE
            };

            void SetVolume(float p_volume);
            void SetLooped(bool p_looped);
            void SetPitch(float p_pitch);
            void SetPan(float p_pan);
            void SetAttenuationModel(EAttenuationModel p_model, float p_factor = 1.0f);
            void SetAttenuationThreshold(float p_distance);

            void SetSpatialParameters(
                const VE::Internal::Math::VVector3& p_position,
                const VE::Internal::Math::VVector3& p_velocity
            ) const;

            void Play();
            void Pause();
            void Stop();

            bool IsValid() const;
            bool IsPaused() const;
            bool IsSpatial() const;

            VSoundHandle GetHandle() const;

        private:
            void Validate() const;

        private:
            SoLoud::Soloud& m_backend;
            VSoundHandle m_handle;
            bool m_spatial;

    };
} // namespace VE::Internal::SoundEngine