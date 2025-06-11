#pragma once

#include "CoreTypes.h"


struct GenericPlatformTime{

	static EFCORE_API double InitTiming();

	static inline double GetSeconds(){
	};

	static float GetTime(){
	}

	static float ToMilliseconds(const uint32 Cycles){
		return static_cast<float>(static_cast<double>(SecondsPerCycle * 1000.0 * Cycles));
	}

	/** Converts cycles to seconds. */
	static float ToSeconds(const uint32 Cycles){
		return static_cast<float>(static_cast<double>(SecondsPerCycle * Cycles));
	}
};

typedef GenericPlatformTime EFPlatformTime;
