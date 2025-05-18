#include <iostream>

#include <EFLogger.h>
#include <Version.h>
#include <GLFW/glfw3.h>

int main(){
    // Initialize the engine logger.
    EventfulEngine::Debug::EFLogger::Init();
    EventfulEngine::Debug::EFLogger::Log(EventfulEngine::Debug::LogLevel::Info, "Initializing...");

    std::cout << "Welcome to EventfulEngine version "
        << EventfulEngine::GetVersion();

    // Initialize GLFW (for window management/rendering)
    if (!::glfwInit()){
        EventfulEngine::Debug::EFLogger::Log(EventfulEngine::Debug::LogLevel::Error, "Failed to initialize GLFW!");
        return -1;
    }

    // Create a simple window.
    GLFWwindow* window = ::glfwCreateWindow(640, 480, "Test Window", nullptr, nullptr);
    if (!window){
        EventfulEngine::Debug::EFLogger::Log(EventfulEngine::Debug::LogLevel::Error, "Failed to create GLFW window!");
        ::glfwTerminate();
        return -1;
    }
    EventfulEngine::Debug::EFLogger::Log(EventfulEngine::Debug::LogLevel::Info, "Window Initialized!");

    // Start Loops Here

    // Cleanup.
    ::glfwDestroyWindow(window);
    ::glfwTerminate();

    return 0;
}
