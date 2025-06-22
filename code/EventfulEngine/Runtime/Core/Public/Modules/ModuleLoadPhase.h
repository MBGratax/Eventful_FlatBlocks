#pragma once

namespace EventfulEngine{

    enum class E_ModuleLoadPhase{
        PreInit = 0u,
        Core = 1u,
        Startup = 2u,
        EditorStartup = 3u,
        Editor = 4u,
        GameSessionInit = 5u,
        GameSession = 6u
    };

}
