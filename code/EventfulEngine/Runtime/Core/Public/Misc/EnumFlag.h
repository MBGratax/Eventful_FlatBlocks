#pragma once

/*! Defines all bitwise operators for enum classes, so it can be used as a regular flags enum. Use this macro after declaring an enum class, and assign enum values bitwise, i.e. ENUMVALUE = 1<<0 etc.
 * @brief Defines all bitwise operators for enum classes, so it can be (mostly) used as a regular flags enum.
 * @param Enum The class name of an Enum class
 */
#define MAKEFLAG(Enum) \
	inline           Enum& operator|=(Enum& Lhs, Enum Rhs) { return Lhs = (Enum)((__underlying_type(Enum))Lhs | (__underlying_type(Enum))Rhs); } \
	inline           Enum& operator&=(Enum& Lhs, Enum Rhs) { return Lhs = (Enum)((__underlying_type(Enum))Lhs & (__underlying_type(Enum))Rhs); } \
	inline           Enum& operator^=(Enum& Lhs, Enum Rhs) { return Lhs = (Enum)((__underlying_type(Enum))Lhs ^ (__underlying_type(Enum))Rhs); } \
	inline constexpr Enum  operator| (Enum  Lhs, Enum Rhs) { return (Enum)((__underlying_type(Enum))Lhs | (__underlying_type(Enum))Rhs); }		 \
	inline constexpr Enum  operator& (Enum  Lhs, Enum Rhs) { return (Enum)((__underlying_type(Enum))Lhs & (__underlying_type(Enum))Rhs); }		 \
	inline constexpr Enum  operator^ (Enum  Lhs, Enum Rhs) { return (Enum)((__underlying_type(Enum))Lhs ^ (__underlying_type(Enum))Rhs); }		 \
	inline constexpr bool  operator! (Enum  E)             { return !(__underlying_type(Enum))E; }												 \
	inline constexpr Enum  operator~ (Enum  E)             { return (Enum)~(__underlying_type(Enum))E; }

/*!
 * @brief Friends all bitwise operators for enum classes so the definition can be kept private / protected.
 * @param Enum The class name of an enum class.
 */
#define FRIEND_MAKEFLAG(Enum)																							\
	friend           Enum& operator|=(Enum& Lhs, Enum Rhs);																\
	friend           Enum& operator&=(Enum& Lhs, Enum Rhs);																\
	friend           Enum& operator^=(Enum& Lhs, Enum Rhs);																\
	friend constexpr Enum  operator| (Enum  Lhs, Enum Rhs);																\
	friend constexpr Enum  operator& (Enum  Lhs, Enum Rhs);																\
	friend constexpr Enum  operator^ (Enum  Lhs, Enum Rhs);																\
	friend constexpr bool  operator! (Enum  E);																			\
	friend constexpr Enum  operator~ (Enum  E);

template <typename Enum>
constexpr bool EnumHasAllFlags(Enum Flags, Enum Contains){
	using UnderlyingType = __underlying_type(Enum);
	return (static_cast<UnderlyingType>(Flags) & static_cast<UnderlyingType>(Contains)) == static_cast<UnderlyingType>(
		Contains);
}

template <typename Enum>
constexpr bool EnumHasAnyFlags(Enum Flags, Enum Contains){
	using UnderlyingType = __underlying_type(Enum);
	return (static_cast<UnderlyingType>(Flags) & static_cast<UnderlyingType>(Contains)) != 0;
}

template <typename Enum>
void EnumAddFlags(Enum& Flags, Enum FlagsToAdd){
	using UnderlyingType = __underlying_type(Enum);
	Flags = static_cast<Enum>(static_cast<UnderlyingType>(Flags) | static_cast<UnderlyingType>(FlagsToAdd));
}

template <typename Enum>
void EnumRemoveFlags(Enum& Flags, Enum FlagsToRemove){
	using UnderlyingType = __underlying_type(Enum);
	Flags = static_cast<Enum>(static_cast<UnderlyingType>(Flags) & ~static_cast<UnderlyingType>(FlagsToRemove));
}
