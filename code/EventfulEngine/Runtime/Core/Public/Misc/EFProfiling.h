#pragma once

//TODO: Add define that disables profiling in distribution
#define EF_ENABLE_PROFILING 1

#if EF_ENABLE_PROFILING
#include <tracy/Tracy.hpp>
#endif

#if EF_ENABLE_PROFILING
#define EF_PROFILE_MARK_FRAME			FrameMark;
#define EF_PROFILE_FUNC(...)			ZoneScoped##__VA_OPT__(N(__VA_ARGS__))
#define EF_PROFILE_SCOPE(...)			EF_PROFILE_FUNC(__VA_ARGS__)
#define EF_PROFILE_SCOPE_DYNAMIC(NAME)  ZoneScoped; ZoneName(NAME, strlen(NAME))
#define EF_PROFILE_THREAD(...)          tracy::SetThreadName(__VA_ARGS__)
#else
#define EF_PROFILE_MARK_FRAME
#define EF_PROFILE_FUNC(...)
#define EF_PROFILE_SCOPE(...)
#define EF_PROFILE_SCOPE_DYNAMIC(NAME)
#define EF_PROFILE_THREAD(...)
#endif
