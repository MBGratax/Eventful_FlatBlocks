#include <iostream>

#include <Core/Version.h>

int main() {
    std::cout << "Welcome to GameEngineProject version "
              << EventfulEngine::GetVersion()
              << std::endl;
    return 0;
}
