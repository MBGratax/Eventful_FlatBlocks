#pragma once
#include<cstdint>

namespace EventfulEngine::Rendering{
    class IShaderModule;

    struct PipelineStateDescriptor{
        IShaderModule* vertexShader;
        IShaderModule* fragmentShader;
    };

    class IPipelineState{
    public:
        virtual ~IPipelineState() = default;

        virtual void GetBindPoint(uint32_t& outBindPoint) const = 0;
    };
}
