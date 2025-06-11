#pragma once

namespace EventfulEngine::Rendering{
    struct RenderPassDescriptor{
    };

    class IRenderPass{
    public:
        virtual ~IRenderPass() = default;
    };
}
