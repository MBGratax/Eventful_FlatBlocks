#pragma once

#include "EFCommandline.h"

namespace EventfulEngine{

    void EFCommandLine::Parse(){
        _options = _optionsParser.parse(Argc(), _argv.data());
    }

    template <typename T>
    T EFCommandLine::GetOption(const EFString& name){
        return _options[name].as<T>();
    }

    EFCommandLine EFCommandLine::Init(const int argc, char** argv){
        _argv.assign(argv, argv + argc);
        return *this;
    }

    EFCommandLine EFCommandLine::Init(const int argc, const char* argv[]){
        _argv.assign(argv, argv + argc);
        return *this;
    }
} // EventfulEngine
