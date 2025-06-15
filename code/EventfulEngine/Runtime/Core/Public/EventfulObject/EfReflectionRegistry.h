#pragma once
#include "EFClass.h"
#include "IManager.h"

namespace EventfulEngine {
// Global Registry for Eventful Reflection, holds all Reflection data that was registered and provides methods to
// get info on classes or invoke methods/get members
class EFReflectionRegistry : public IManager{

};

} // EventfulEngine
