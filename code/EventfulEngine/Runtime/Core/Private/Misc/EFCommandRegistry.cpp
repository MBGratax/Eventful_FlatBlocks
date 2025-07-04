#pragma once

#include "EfCommandRegistry.h"

namespace EventfulEngine{
    static std::unordered_map<EFString, EFCommandFunction> g_commands;

    void EFCommandRegistry::Register(const EFString& name, EFCommandFunction func){
        g_commands[name] = std::move(func);
    }

    void EFCommandRegistry::Unregister(const EFString& name){
        g_commands.erase(name);
    }

    bool EFCommandRegistry::Execute(const EFString& commandLine){
        std::istringstream stringStream(commandLine);
        EFString cmd;
        if (!(stringStream >> cmd)){
            return false;
        }
        const auto iterator = g_commands.find(cmd);
        if (iterator == g_commands.end()){
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
