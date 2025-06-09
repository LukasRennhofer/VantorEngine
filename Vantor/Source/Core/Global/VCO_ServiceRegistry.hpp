#pragma once

// RenderDevice
#include "../../RenderDevice/Interface/VRD_RenderDevice.hpp"

namespace Vantor {
    class VServiceLocator {
    public:
        static void SetRenderDevice(RenderDevice::VRDevice* dev) { renderDevice = dev; }
        static RenderDevice::VRDevice* GetRenderDevice() { return renderDevice; }

        // TODO: Add more systems here
    private:
        static inline RenderDevice::VRDevice* renderDevice = nullptr;
    };
}
