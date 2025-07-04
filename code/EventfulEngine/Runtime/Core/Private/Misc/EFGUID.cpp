#pragma once

#include "EFGUID.h"
#include "Platform.h"

#if EF_PLATFORM_LINUX
#include <uuid/uuid.h>
#endif

#if EF_PLATFORM_APPLE
#include <CoreFoundation/CFUUID.h>
#endif

#if EF_PLATFORM_WINDOWS
#include <objbase.h>
#endif

namespace EventfulEngine{
	// overload << so that it's easy to convert to a string
	std::ostream& operator<<(std::ostream& s, const EFGUID& guid){
		return s << guid.String();
	}

	bool EFGUID::IsValid() const{
		const EFGUID empty;
		return *this != empty;
	}

	EFString EFGUID::String() const{
		std::array<char, 10> one{};
		std::array<char, 6> two{};
		std::array<char, 6> three{};
		std::array<char, 6> four{};
		std::array<char, 14> five{};

		std::format_to_n(one.begin(), one.size(), "{:02x}{:02x}{:02x}{:02x}", _bytes[0], _bytes[1], _bytes[2],
		                 _bytes[3]);
		std::format_to_n(two.begin(), 6, "{:02x}{:02x}", _bytes[4], _bytes[5]);
		std::format_to_n(three.begin(), 6, "{:02x}{:02x}", _bytes[6], _bytes[7]);
		std::format_to_n(four.begin(), 6, "{:02x}{:02x}", _bytes[8], _bytes[9]);
		std::format_to_n(five.begin(), 14, "{:02x}{:02x}{:02x}{:02x}{:02x}{:02x}", _bytes[10], _bytes[11], _bytes[12],
		                 _bytes[13], _bytes[14],
		                 _bytes[15]);
		const EFString sep("-");
		EFString out(one.data());

		out += sep + two.data();
		out += sep + three.data();
		out += sep + four.data();
		out += sep + five.data();

		return out;
	}

	// conversion operator for std::string
	EFGUID::operator std::string() const{
		return String();
	}

	// Access underlying bytes
	const std::array<unsigned char, 16>& EFGUID::Bytes() const{
		return _bytes;
	}

	// create a guid from vector of bytes
	EFGUID::EFGUID(const std::array<unsigned char, 16>& bytes) : _bytes(bytes){
	}

	// create a guid from vector of bytes
	EFGUID::EFGUID(std::array<unsigned char, 16>&& bytes) : _bytes(std::move(bytes)){
	}

	// converts a single hex char to a number (0-15)
	unsigned char HexDigitToChar(const char ch){
		// 0-9
		if (ch > 47 && ch < 58){
			return ch - 48;
		}

		// a-f
		if (ch > 96 && ch < 103){
			return ch - 87;
		}

		// A-F
		if (ch > 64 && ch < 71){
			return ch - 55;
		}

		return 0;
	}

	bool IsValidHexChar(const char ch){
		// 0-9
		if (ch > 47 && ch < 58){
			return true;
		}

		// a-f
		if (ch > 96 && ch < 103){
			return true;
		}

		// A-F
		if (ch > 64 && ch < 71){
			return true;
		}

		return false;
	}

	// converts the two hexadecimal characters to an unsigned char (a byte)
	unsigned char HexPairToChar(char a, char b){
		return HexDigitToChar(a) * 16 + HexDigitToChar(b);
	}

	// create a guid from string
	EFGUID::EFGUID(const std::string_view fromString){
		char charOne = '\0';
		char charTwo = '\0';
		bool lookingForFirstChar = true;
		unsigned nextByte = 0;

		for (const char& ch : fromString){
			if (ch == '-')
				continue;

			if (nextByte >= 16 || !IsValidHexChar(ch)){
				// Invalid string so bail
				Invalidate();
				return;
			}

			if (lookingForFirstChar){
				charOne = ch;
				lookingForFirstChar = false;
			}
			else{
				charTwo = ch;
				const auto byte = HexPairToChar(charOne, charTwo);
				_bytes[nextByte++] = byte;
				lookingForFirstChar = true;
			}
		}

		// if there were fewer than 16 bytes in the string, then guid is bad
		if (nextByte < 16){
			Invalidate();
			return;
		}
	}

	// create empty guid
	EFGUID::EFGUID() : _bytes{{0}}{
	}

	// set all bytes to zero
	void EFGUID::Invalidate(){
		std::ranges::fill_n(_bytes.begin(), _bytes.size(), static_cast<unsigned char>(0));
	}

	// member swap function
	void EFGUID::Swap(EFGUID& other){
		_bytes.swap(other._bytes);
	}


#if EF_PLATFORM_LINUX
	Guid newGuid()
	{
		std::array<unsigned char, 16> data;
		static_assert(std::is_same<unsigned char[16], uuid_t>::value, "Wrong type!");
		uuid_generate(data.data());
		return Guid{std::move(data)};
	}
#endif


#if EF_PLATFORM_APPLE
	Guid newGuid()
	{
		auto newId = CFUUIDCreate(NULL);
		auto bytes = CFUUIDGetUUIDBytes(newId);
		CFRelease(newId);

		std::array<unsigned char, 16> byteArray =
		{{
			bytes.byte0,
			bytes.byte1,
			bytes.byte2,
			bytes.byte3,
			bytes.byte4,
			bytes.byte5,
			bytes.byte6,
			bytes.byte7,
			bytes.byte8,
			bytes.byte9,
			bytes.byte10,
			bytes.byte11,
			bytes.byte12,
			bytes.byte13,
			bytes.byte14,
			bytes.byte15
		}};
		return Guid{std::move(byteArray)};
	}
#endif


#if EF_PLATFORM_WINDOWS
	EFGUID NewGuid(){
		GUID newId;
		CoCreateGuid(&newId);

		const std::array bytes =
			{
				static_cast<unsigned char>((newId.Data1 >> 24) & 0xFF),
				static_cast<unsigned char>((newId.Data1 >> 16) & 0xFF),
				static_cast<unsigned char>((newId.Data1 >> 8) & 0xFF),
				static_cast<unsigned char>((newId.Data1) & 0xff),

				static_cast<unsigned char>((newId.Data2 >> 8) & 0xFF),
				static_cast<unsigned char>((newId.Data2) & 0xff),

				static_cast<unsigned char>((newId.Data3 >> 8) & 0xFF),
				static_cast<unsigned char>((newId.Data3) & 0xFF),

				newId.Data4[0],
				newId.Data4[1],
				newId.Data4[2],
				newId.Data4[3],
				newId.Data4[4],
				newId.Data4[5],
				newId.Data4[6],
				newId.Data4[7]
			};

		return EFGUID{bytes};
	}
#endif
}
