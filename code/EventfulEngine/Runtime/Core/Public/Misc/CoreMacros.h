#pragma once

#define NOMOVE(ClassName)                           \
ClassName& operator=(ClassName&& other) = delete;

#define NOCOPY(ClassName)                           \
ClassName& operator=(ClassName& other) = delete;

#define NOMOVEORCOPY(ClassName)                           \
ClassName& operator=(ClassName&& other) = delete;         \
ClassName& operator=(ClassName& other) = delete;

// TODO: Make global via cmake
#define EVENTFUL_PLATFORM Windows

// Macro Expansion into stringify
#define EF_STRINGIFY(Token) STRINGIFY(Token)
#define STRINGIFY(Name) #Name
// Macro expansion, then concatenates
#define EF_CONCAT(TokenA, TokenB) CONCAT(TokenA, TokenB)
#define CONCAT(TokenA,TokenB) TokenA##TokenB

// Results in a header in the style of "Windows/Windows{Name}"
#define CONCATPLATFORMHEADER(Name) EF_STRINGIFY(EF_CONCAT(EVENTFUL_PLATFORM/EVENTFUL_PLATFORM,Name))
