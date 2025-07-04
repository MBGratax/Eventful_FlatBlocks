#pragma once

#include "EFDynamicRAPI.h"

namespace EventfulEngine{
    extern DynamicRAPIHandle g_dynamicRAPI;

    class RenderAPIModule final : public IModule{
    public:
        ~RenderAPIModule() override = default;

        void ShutdownModule() override;

        bool SupportsDynamicReloading() override;

        E_ModuleLoadPhase GetLoadPhase() override{
            return E_ModuleLoadPhase::Startup;
        }

        void InitializeDynamicRAPI(E_RenderAPI api);

    private:
        IDynamicRAPIModule* _currentRAPI = nullptr;
        EFString _currentRAPIName;
    };
}
