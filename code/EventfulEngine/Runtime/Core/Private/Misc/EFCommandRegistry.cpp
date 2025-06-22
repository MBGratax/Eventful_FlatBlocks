#pragma once

#include "EfCommandRegistry.h"

namespace EventfulEngine{
    static std::unordered_map<EFString, EFCommandFunction> g_Commands;

    void EFCommandRegistry::Register(const EFString& name, EFCommandFunction func){
        g_Commands[name] = std::move(func);
    }

    void EFCommandRegistry::Unregister(const EFString& name){
        g_Commands.erase(name);
    }

    bool EFCommandRegistry::Execute(const EFString& commandLine){
        std::istringstream stringStream(commandLine);
        EFString cmd;
        if (!(stringStream >> cmd)){
            return false;
        }
        const auto iterator = g_Commands.find(cmd);
        if (iterator == g_Commands.end()){
            return false;
        }

        std::vector<EFString> args;
        EFString arg;
        while (stringStream >> arg){
            args.push_back(arg);
        }

        iterator->second(args);
        return true;
    }
} // EventfulEngine
