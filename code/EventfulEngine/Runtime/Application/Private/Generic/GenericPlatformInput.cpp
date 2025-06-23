#pragma once

#include "GenericPlatformInput.h"

namespace EventfulEngine{
    bool GenericPlatformInput::IsGamepadConnected(const int index){
        return ::glfwJoystickIsGamepad(index) == GLFW_TRUE;
    }

    bool GenericPlatformInput::GetGamepadState(const int index, GLFWgamepadstate& state){
        return ::glfwGetGamepadState(index, &state) == GLFW_TRUE;
    }

    bool GenericPlatformInput::IsKeyPressed(GLFWwindow* window, const int key){
        return window && ::glfwGetKey(window, key) == GLFW_PRESS;
    }

    bool GenericPlatformInput::IsMouseButtonPressed(GLFWwindow* window, const int button){
        return window && ::glfwGetMouseButton(window, button) == GLFW_PRESS;
    }

    void GenericPlatformInput::GetCursorPos(GLFWwindow* window, double& x, double& y){
        if (window){
            ::glfwGetCursorPos(window, &x, &y);
        }
    }
}
