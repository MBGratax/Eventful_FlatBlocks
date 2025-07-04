#pragma once
#include <memory>
#include <mutex>
#include <map>
#include "CoreMacros.h"
// TODO: Wrap std smartpointers
// TODO: Optional; Custom Memory Alignment and management, overwrite global standard new and delete keywords to use
// threshold based Garbage Collection.
namespace EventfulEngine{

    DECLARE_HANDLE(EFMemoryHandle);

    struct EFAllocationStats{
        size_t TotalAllocated = 0;
        size_t TotalFreed = 0;
    };

    struct EFAllocation{
        void* Memory = nullptr;
        size_t Size = 0;
        const char* Category = nullptr;
    };

    namespace EFMemory{
        const EFAllocationStats& GetAllocationStats();
    }

    template <class T>
    struct EFMallocator{
        typedef T value_type;

        EFMallocator() = default;

        template <class U>
        explicit(false) constexpr EFMallocator(const EFMallocator<U>&) noexcept{
        }

        T* allocate(const std::size_t n){
#undef max
            if (n > std::numeric_limits<std::size_t>::max() / sizeof(T))
                throw std::bad_array_new_length();

            if (auto p = static_cast<T*>(std::malloc(n * sizeof(T)))){
                return p;
            }

            throw std::bad_alloc();
        }

        static void deallocate(T* p, std::size_t n) noexcept{
            std::free(p);
        }
    };

    struct AllocatorData{
        using MapAlloc = EFMallocator<std::pair<const void* const, EFAllocation>>;
        using StatsMapAlloc = EFMallocator<std::pair<const char* const, EFAllocationStats>>;

        using AllocationStatsMap = std::map<const char*, EFAllocationStats, std::less<const char*>, StatsMapAlloc>;

        std::map<const void*, EFAllocation, std::less<const void*>, MapAlloc> AllocationMap;
        AllocationStatsMap AllocationStatsMap_;

        std::mutex Mutex, StatsMutex;
    };


    class EFAllocator{
    public:
        static void Init();

        static EFMemoryHandle AllocateRaw(size_t size);

        static EFMemoryHandle Allocate(size_t size);

        static EFMemoryHandle Allocate(size_t size, const char* desc);

        static EFMemoryHandle Allocate(size_t size, const char* file, int line);

        static void Free(EFMemoryHandle memory);

        static const AllocatorData::AllocationStatsMap& GetAllocationStats(){ return _data->AllocationStatsMap_; }

    private:
        inline static AllocatorData* _data = nullptr;
    };
}

_NODISCARD _Ret_notnull_ _Post_writable_byte_size_(size) _VCRT_ALLOCATOR
void* __CRTDECL operator new(size_t _Size);

_NODISCARD _Ret_notnull_ _Post_writable_byte_size_(size) _VCRT_ALLOCATOR
void* __CRTDECL operator new[](size_t _Size);

_NODISCARD _Ret_notnull_ _Post_writable_byte_size_(size) _VCRT_ALLOCATOR
void* __CRTDECL operator new(size_t size, const char* desc);

_NODISCARD _Ret_notnull_ _Post_writable_byte_size_(size) _VCRT_ALLOCATOR
void* __CRTDECL operator new[](size_t size, const char* desc);

_NODISCARD _Ret_notnull_ _Post_writable_byte_size_(size) _VCRT_ALLOCATOR
void* __CRTDECL operator new(size_t size, const char* file, int line);

_NODISCARD _Ret_notnull_ _Post_writable_byte_size_(size) _VCRT_ALLOCATOR
void* __CRTDECL operator new[](size_t size, const char* file, int line);

void __CRTDECL operator delete(void* _Block) noexcept;

void __CRTDECL operator delete(void* _Block, size_t _Size) noexcept;

void __CRTDECL operator delete(void* memory, const char* desc) noexcept;

void __CRTDECL operator delete(void* memory, const char* file, int line) noexcept;

void __CRTDECL operator delete[](void* _Block) noexcept;

void __CRTDECL operator delete[](void* _Block, size_t _Size) noexcept;

void __CRTDECL operator delete[](void* memory, const char* desc) noexcept;

void __CRTDECL operator delete[](void* memory, const char* file, int line) noexcept;
