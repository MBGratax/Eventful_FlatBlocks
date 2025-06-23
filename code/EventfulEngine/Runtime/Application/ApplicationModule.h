#pragma once

#include "IModule.h"
#include "PlatformApplication.h"

// Entrypoint and setup for module interface
namespace EventfulEngine{

    class ApplicationModule final : public IModule{
    public:
        ApplicationModule() = default;

        ~ApplicationModule() override = default;

        void StartupModule() override;

        void ShutdownModule() override;

        EFApplication& GetApplication(){ return _application; }

    private:
        EFApplication _application;
    };
}
