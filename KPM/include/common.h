#pragma once

#include <filesystem>
#include <algorithm>
#include <iostream>
#include <concepts>
#include <utility>
#include <cstdint>
#include <string>
#include <bitset>
#include <new>
#include <bit>

namespace kpm
{
	namespace fs = std::filesystem;

	typedef std::int8_t  s8;
	typedef std::int16_t s16;
	typedef std::int32_t s32;
	typedef std::int64_t s64;

	typedef std::uint8_t  u8;
	typedef std::uint16_t u16;
	typedef std::uint32_t u32;
	typedef std::uint64_t u64;

	typedef std::size_t size_t;
	typedef std::size_t offset_t;

	typedef u8  BYTE;
	typedef u16 WORD;
	typedef u32 LONG;
	typedef u64 QUAD;

	typedef fs::path Path;
}

namespace kpm::utils
{
	template <typename _Ty>
	concept numeric = std::integral<_Ty> || std::floating_point<_Ty>;
}

namespace kpm::utils
{
	template <typename _Ty = void>
	inline _Ty* malloc(size_t size) { return reinterpret_cast<_Ty*>(::operator new(size)); }

	template <typename _Ty = void>
	inline _Ty* calloc(size_t count, size_t size) { return reinterpret_cast<_Ty*>(::operator new(count * size)); }

	inline void free(void* ptr) { ::operator delete(ptr); }

	template <typename _Ty, typename... _ArgTys> requires std::constructible_from<_Ty, _ArgTys...>
	inline _Ty& construct(_Ty& object, _ArgTys&&... args)
	{
		return *(new (object) _Ty{ std::forward<_ArgTys>(args)... });
	}

	template <std::copy_constructible _Ty>
	inline _Ty& copy_construct(_Ty& dst, const _Ty& src) { return construct(dst, src); }

	template <std::move_constructible _Ty>
	inline _Ty& move_construct(_Ty& dst, _Ty&& src) { return construct(dst, std::move(src)); }

	template <std::destructible _Ty>
	inline _Ty& destroy(_Ty& object) { return object.~_Ty(), object; }

	template <std::integral _Ty, std::convertible_to<_Ty> _MinTy, std::convertible_to<_Ty> _MaxTy> requires
		requires(_Ty x)
		{
			std::max<_Ty>(x);
			std::min<_Ty>(x);
		}
	constexpr _Ty clamp(_Ty value, _MinTy min, _MaxTy max)
	{
		return std::min<_Ty>(static_cast<_Ty>(max), std::max(static_cast<_Ty>(min), value));
	}

	constexpr bool is_little_endian() { return std::endian::native == std::endian::little; }
	constexpr bool is_big_endian() { return std::endian::native == std::endian::big; }

	constexpr void raw_write(void* dst, numeric auto value, offset_t off = 0)
	{
		*reinterpret_cast<decltype(value)*>(reinterpret_cast<u8*>(dst) + off) = value;
	}

	template <typename _Ty>
	constexpr void raw_write(void* dst, const _Ty& data, offset_t off = 0)
	{
		*reinterpret_cast<_Ty*>(reinterpret_cast<u8*>(dst) + off) = data;
	}

	constexpr void raw_read(numeric auto& value, const void* src, offset_t off = 0)
	{
		value = *reinterpret_cast<const decltype(value)*>(reinterpret_cast<const u8*>(src) + off);
	}

	template <typename _Ty>
	constexpr void raw_read(_Ty& data, const void* src, offset_t off = 0)
	{
		data = *reinterpret_cast<const _Ty*>(reinterpret_cast<const u8*>(src) + off);
	}
}
