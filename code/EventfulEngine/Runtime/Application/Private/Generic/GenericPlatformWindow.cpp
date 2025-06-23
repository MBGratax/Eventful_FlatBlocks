#pragma once

#include "GenericPlatformWindow.h"


namespace EventfulEngine{
    GLFWwindow* GenericPlatformWindow::CreateWindow(const int width, const int height, const char* title){
        return ::glfwCreateWindow(width, height, title, nullptr, nullptr);
    }

    void GenericPlatformWindow::DestroyWindow(GLFWwindow* window){
        if (window){
            ::glfwDestroyWindow(window);
        }
    }

    void GenericPlatformWindow::PollEvents(){
        ::glfwPollEvents();
    }

} // namespace EventfulEngine
