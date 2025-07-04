#pragma once

#include "EfMemory.h"

#include <CoreGlobals.h>

#include "EFLogger.h"
#include "EFProfiling.h"
#include <stdexcept>

namespace EventfulEngine{

	static EFAllocationStats g_globalStats;

	static bool g_bIsInInit = false;

	void EFAllocator::Init(){
		if (_data)
			return;

		g_bIsInInit = true;
		auto* data = static_cast<AllocatorData*>(static_cast<void*>(AllocateRaw(sizeof(AllocatorData))));
		new(data) AllocatorData();
		_data = data;
		g_bIsInInit = false;
	}

	EFMemoryHandle EFAllocator::AllocateRaw(const size_t size){
		return ::malloc(size);
	}

	EFMemoryHandle EFAllocator::Allocate(size_t size){
		if (g_bIsInInit){
			return AllocateRaw(size);
		}

		if (!_data){
			Init();
		}

		const EFMemoryHandle memory = ::malloc(size);

		{
			std::scoped_lock lock(_data->Mutex);
			EFAllocation& alloc = _data->AllocationMap[memory];
			alloc.Memory = memory;
			alloc.Size = size;
			g_globalStats.TotalAllocated += size;
		}

#if EF_ENABLE_PROFILING
		TracyAlloc(memory, size);
#endif

		return memory;
	}

	EFMemoryHandle EFAllocator::Allocate(const size_t size, const char* desc){
		if (!_data){
			Init();
		}

		const EFMemoryHandle memory = ::malloc(size);

		{
			std::scoped_lock lock(_data->Mutex);
			auto& [Memory, Size, Category] = _data->AllocationMap[memory];
			Memory = memory;
			Size = size;
			Category = desc;

			g_globalStats.TotalAllocated += size;
			if (desc)
				_data->AllocationStatsMap_[desc].TotalAllocated += size;
		}

#if EF_ENABLE_PROFILING
		TracyAlloc(memory, size);
#endif

		return memory;
	}

	EFMemoryHandle EFAllocator::Allocate(const size_t size, const char* file, [[maybe_unused]] int line){
		if (!_data){
			Init();
		}

		void* memory = ::malloc(size);

		{
			std::scoped_lock lock(_data->Mutex);
			auto& [Memory, Size, Category] = _data->AllocationMap[memory];
			Memory = memory;
			Size = size;
			Category = file;

			g_globalStats.TotalAllocated += size;
			_data->AllocationStatsMap_[file].TotalAllocated += size;
		}

#if EF_ENABLE_PROFILING
		TracyAlloc(memory, size);
#endif

		return memory;
	}

	void EFAllocator::Free(EFMemoryHandle memory){
		if (memory == nullptr){
			return;
		}

		if (!_data){
			Init();
		}

		{
			bool found = false;
			{
				std::scoped_lock lock(_data->Mutex);
				const auto allocMapIt = _data->AllocationMap.find(memory);
				found = allocMapIt != _data->AllocationMap.end();
				if (found){
					const EFAllocation& alloc = allocMapIt->second;
					g_globalStats.TotalFreed += alloc.Size;

					if (alloc.Category){
						_data->AllocationStatsMap_[alloc.Category].TotalFreed += alloc.Size;
					}

					_data->AllocationMap.erase(memory);
				}
			}

#if EF_ENABLE_PROFILING
			TracyFree(memory);
#endif

#ifndef EF_DISTRIBUTED
			if (!found){
				EF_WARN_CAT(CoreLog,
				            "Memory block {} not present in alloc map", memory.ptr);
			}
#endif
		}

		::free(memory);
	}

	namespace Memory{
		const EFAllocationStats& GetAllocationStats(){ return g_globalStats; }
	}}


_NODISCARD _Ret_notnull_ _Post_writable_byte_size_(size) _VCRT_ALLOCATOR
void* __CRTDECL operator new(const size_t _Size){
	return EventfulEngine::EFAllocator::Allocate(_Size);
}

_NODISCARD _Ret_notnull_ _Post_writable_byte_size_(size) _VCRT_ALLOCATOR
void* __CRTDECL operator new[](const size_t _Size){
	return EventfulEngine::EFAllocator::Allocate(_Size);
}

_NODISCARD _Ret_notnull_ _Post_writable_byte_size_(size) _VCRT_ALLOCATOR
void* __CRTDECL operator new(const size_t size, const char* desc){
	return EventfulEngine::EFAllocator::Allocate(size, desc);
}

_NODISCARD _Ret_notnull_ _Post_writable_byte_size_(size) _VCRT_ALLOCATOR
void* __CRTDECL operator new[](const size_t size, const char* desc){
	return EventfulEngine::EFAllocator::Allocate(size, desc);
}

_NODISCARD _Ret_notnull_ _Post_writable_byte_size_(size) _VCRT_ALLOCATOR
void* __CRTDECL operator new(const size_t size, const char* file, const int line){
	return EventfulEngine::EFAllocator::Allocate(size, file, line);
}

_NODISCARD _Ret_notnull_ _Post_writable_byte_size_(size) _VCRT_ALLOCATOR
void* __CRTDECL operator new[](const size_t size, const char* file, const int line){
	return EventfulEngine::EFAllocator::Allocate(size, file, line);
}

void __CRTDECL operator delete(void* _Block) noexcept{
	return EventfulEngine::EFAllocator::Free(_Block);
}

void __CRTDECL operator delete(void* _Block, [[maybe_unused]] size_t _Size) noexcept{
	return EventfulEngine::EFAllocator::Free(_Block);
}

void __CRTDECL operator delete(void* memory, [[maybe_unused]] const char* desc){
	return EventfulEngine::EFAllocator::Free(memory);
}

void __CRTDECL operator delete(void* memory, [[maybe_unused]] const char* file, [[maybe_unused]] int line){
	return EventfulEngine::EFAllocator::Free(memory);
}

void __CRTDECL operator delete[](void* _Block){
	return EventfulEngine::EFAllocator::Free(static_cast<EventfulEngine::EFMemoryHandle>(_Block));
}

void __CRTDECL operator delete[](void* _Block, [[maybe_unused]] size_t _Size){
	return EventfulEngine::EFAllocator::Free(static_cast<EventfulEngine::EFMemoryHandle>(_Block));
}

void __CRTDECL operator delete[](void* memory, [[maybe_unused]] const char* desc){
	return EventfulEngine::EFAllocator::Free(static_cast<EventfulEngine::EFMemoryHandle>(memory));
}

void __CRTDECL operator delete[](void* memory, [[maybe_unused]] const char* file, [[maybe_unused]] int line){
	return EventfulEngine::EFAllocator::Free(static_cast<EventfulEngine::EFMemoryHandle>(memory));
}
