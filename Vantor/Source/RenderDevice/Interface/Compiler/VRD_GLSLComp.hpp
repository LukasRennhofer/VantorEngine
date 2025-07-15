#pragma once

// We use vglsl as our preprocessor
#include <Shared/VGLSL/vglsl.h>

#include "../../../Core/Types/VCO_BTypes.hpp" // VSingleton

#include "../../../Core/BackLog/VCO_Backlog.hpp" // Backlog

#include <vector>
#include <string>

namespace Vantor::RenderDevice {
    class VGLSLPreproccessor : public Vantor::Core::Types::VSingleton<VGLSLPreproccessor>
    {
        friend class Vantor::Core::Types::VSingleton<VGLSLPreproccessor>;

    private:
        std::vector<std::string> m_VirtualIncludes;
    public:

        // Private constructor/destructor
        VGLSLPreproccessor();
        ~VGLSLPreproccessor();

        const char* ProccessFromMemory(const char* memory, const char* fileName);

        std::vector<std::string> GetVirtualIncludes() { return m_VirtualIncludes; }
    };
}

