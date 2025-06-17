#pragma once

#include "EfMemory.h"

#include <stdexcept>

namespace EventfulEngine{
    void* EFMemory::Malloc(std::size_t size){
        return std::malloc(size);
    }

    void* EFMemory::Realloc(void* ptr, std::size_t newSize){
        return std::realloc(ptr, newSize);
    }

    void EFMemory::Free(void* ptr){
        std::free(ptr);
    }

    void* EFMemory::AlignedMalloc(std::size_t size, std::size_t alignment){
        // Placeholder for alignment aware allocation
        // Use standard malloc for now.
        (void)alignment;
        return std::malloc(size);
    }

    void EFMemory::AlignedFree(void* ptr){
        // Placeholder for aligned free handling
        std::free(ptr);
    }

    void EFMemory::CollectGarbage(){
        // Stub for future garbage collection system
    }
}
