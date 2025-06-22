#pragma once
#include <CoreTypes.h>
#include <functional>


namespace EventfulEngine{
    using EFCommandFunction = std::function<void(const std::vector<EFString>&)>;

    class EFCORE_API EFCommandRegistry{

    public:
        static void Register(const EFString& name, EFCommandFunction func);

        static void Unregister(const EFString& name);

        static bool Execute(const EFString& commandLine);
    };
};
