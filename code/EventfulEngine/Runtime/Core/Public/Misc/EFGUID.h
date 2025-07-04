#pragma once

#include <array>
#include <cassert>
#include <functional>
#include <string_view>

#include "CoreTypes.h"

namespace EventfulEngine{
	class EFGUID{
	public:
		explicit EFGUID(const std::array<unsigned char, 16>& bytes);

		explicit EFGUID(std::array<unsigned char, 16>&& bytes);

		explicit EFGUID(std::string_view fromString);

		EFGUID();

		EFGUID(const EFGUID& other) = default;

		EFGUID& operator=(const EFGUID& other) = default;

		EFGUID(EFGUID&& other) = default;

		EFGUID& operator=(EFGUID&& other) = default;

		auto operator<=>(const EFGUID& other) const = default;

		EFString String() const;

		explicit(false) operator EFString() const;

		const std::array<unsigned char, 16>& Bytes() const;

		void Swap(EFGUID& other);

		bool IsValid() const;

	private:
		void Invalidate();

		friend std::ostream& operator<<(std::ostream& s, const EFGUID& guid);

		// actual data
		std::array<unsigned char, 16> _bytes{};

		// make the << operator a friend so it can access _bytes
		friend std::ostream& operator<<(std::ostream& s, const EFGUID& guid);

		friend bool operator<(const EFGUID& lhs, const EFGUID& rhs);
	};

	EFGUID NewGuid();

	namespace Details{
		template <typename...>
		struct Hash;

		template <typename T>
		struct Hash<T> : std::hash<T>{
			using std::hash<T>::hash;
		};


		template <typename T, typename... Rest>
		struct Hash<T, Rest...>{
			std::size_t operator()(const T& v, const Rest&... rest){
				std::size_t seed = Hash<Rest...>{}(rest...);
				seed ^= Hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
				return seed;
			}
		};
	}}

namespace std{
	// Template specialization for std::swap<Guid>() --
	// See guid.cpp for the function definition
	inline void swap(EventfulEngine::EFGUID& lhs, EventfulEngine::EFGUID& rhs) noexcept{
		lhs.Swap(rhs);
	}

	// Specialization for std::hash<Guid> -- this implementation
	// uses std::hash<std::string> on the stringification of the guid
	// to calculate the hash
	template <>
	struct hash<EventfulEngine::EFGUID>{
		std::size_t operator()(EventfulEngine::EFGUID const& guid) const noexcept{
			auto p = reinterpret_cast<const uint64*>(guid.Bytes().data());
			return EventfulEngine::Details::Hash<uint64, uint64>{}(p[0], p[1]);
		}
	};
}
