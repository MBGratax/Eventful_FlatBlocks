#pragma once
#include <memory>

// TODO: Wrap std smartpointers
// TODO: Optional; Custom Memory Alignment and management, overwrite global standard new and delete keywords to use
// threshold based Garbage Collection.
namespace EventfulEngine{

    template <typename T>
    using EFUniquePtr = std::unique_ptr<T>;

    template <typename T>
    using EFSharedPtr = std::shared_ptr<T>;

    template <typename T>
    using EFWeakPtr = std::weak_ptr<T>;

    template <typename T, typename... Args>
    inline EFUniquePtr<T> MakeUnique(Args&&... args);

    template <typename T, typename... Args>
    inline EFSharedPtr<T> MakeShared(Args&&... args);

    class EFMemory{
    public:
        // Allocate "size" bytes of memory.
        static void* Malloc(std::size_t size);

        // Resize previously allocated memory block.
        static void* Realloc(void* ptr, std::size_t newSize);

        // Free memory previously allocated with Malloc/Realloc.
        static void Free(void* ptr);

        // TODO: Provide alignment-aware variants of the above.
        static void* AlignedMalloc(std::size_t size, std::size_t alignment);

        static void AlignedFree(void* ptr);

        // TODO: Implement garbage collection for threshold based cleanup.
        static void CollectGarbage();
    };
}

#include "EfMemory.inl"
