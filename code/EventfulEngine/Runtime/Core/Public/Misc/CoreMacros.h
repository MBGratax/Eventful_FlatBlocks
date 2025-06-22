#pragma once

// TODO: Keep updated with useful macros

#define NOMOVE(ClassName)                           \
    ClassName(ClassName&&)        noexcept = delete;\
    ClassName& operator=(ClassName&&) noexcept = delete;

#define NOCOPY(ClassName)                           \
    ClassName(const ClassName&)            = delete;\
    ClassName& operator=(const ClassName&) = delete;

#define NOMOVEORCOPY(ClassName)                           \
    NOCOPY(ClassName)                                     \
    NOMOVE(ClassName)

// TODO: Make global via cmake
#define EVENTFUL_PLATFORM Windows

// Macro Expansion into stringify
#define EF_STRINGIFY(Token) STRINGIFY(Token)
#define STRINGIFY(Name) #Name
// Macro expansion, then concatenates
#define EF_CONCAT(TokenA, TokenB) CONCAT(TokenA, TokenB)
#define CONCAT(TokenA,TokenB) TokenA##TokenB

// Copy of Windows style handles to make handles typesafe(r)
#define DECLARE_HANDLE(Name) struct Name##__{int unused;}; typedef struct Name##__ *Name

// Results in a header in the style of "Windows/Windows{Name}"
#define CONCATPLATFORMHEADER(Name) EF_STRINGIFY(EF_CONCAT(EVENTFUL_PLATFORM/EVENTFUL_PLATFORM,Name))
