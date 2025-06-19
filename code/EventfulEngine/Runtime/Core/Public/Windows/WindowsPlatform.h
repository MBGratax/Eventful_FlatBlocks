#pragma once

// Definition for EFGenericPlatformTypes
#include "GenericPlatform/GenericPlatform.h"

// Simple override struct for windows specific types (Currently placeholder)
struct EFWindowsPlatformTypes : public EFGenericPlatformTypes{
};

typedef EFWindowsPlatformTypes EFPlatformTypes;

// DLL export and import definitions
#define DLLEXPORT __declspec(dllexport)
#define DLLIMPORT __declspec(dllimport)

