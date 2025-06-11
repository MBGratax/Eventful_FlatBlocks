#pragma once

namespace EventfulEngine::Rendering{
    class IDescriptorSet{
    public:
        virtual ~IDescriptorSet() = default;

        virtual void UpdateDescriptors() = 0;
    };
}
