#pragma once
#include<cstdint>

namespace EventfulEngine::Rendering{
    class IShaderModule{
    public:
        virtual ~IShaderModule() = default;

        virtual uint32_t GetStageFlags() const = 0;
    };
}
