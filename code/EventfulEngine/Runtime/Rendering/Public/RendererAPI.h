#pragma once

namespace EventfulEngine::Rendering{
    enum class RendererAPI{
        Headless = 0,
        D3D12 = 1 << 0,
        Vulkan = 1 << 1,
        OpenGL = 1 << 2,
        Metal = 1 << 3
    };
}
