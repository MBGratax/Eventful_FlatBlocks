#pragma once

#include "Thread.h"

namespace EventfulEngine{
    Thread::~Thread(){
        if (_thread.joinable()){
            _thread.detach();
        }
    }

    void Thread::Join(){
        if (_thread.joinable()){
            _thread.join();
        }
    }

    bool Thread::Joinable() const noexcept{
        return _thread.joinable();
    }
} // EventfulEngine
