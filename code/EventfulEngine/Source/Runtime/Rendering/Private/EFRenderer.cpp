#pragma once

#include "EFRenderer.h"

#include <bit>
#include <iostream>
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace EventfulEngine::Rendering{
    bool EFRenderer::Init(){
        // Initialize glad using GLFW's procedure to get function addresses.
        if (!::gladLoadGLLoader(std::bit_cast<GLADloadproc, GLFWglproc>(::glfwGetProcAddress))){
            std::cerr << "Failed to initialize GLAD\n";
            return false;
        }
        return true;
    }
}
