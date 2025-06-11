#pragma once
#include<memory>

namespace EventfulEngine::Rendering{
    class IDescriptorSet;

    class IDescriptorPool{
    public:
        virtual ~IDescriptorPool() = default;

        virtual std::unique_ptr<IDescriptorSet> Allocate() = 0;
    };
}
