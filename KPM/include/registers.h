#pragma once

#include "common.h"

namespace kpm
{
	union Register
	{
		BYTE b;
		WORD w;
		LONG l;
		QUAD q = 0;

	public:
		Register() = default;
		Register(const Register&) = default;
		Register(Register&&) noexcept = default;
		~Register() = default;

		Register& operator= (const Register&) = default;
		Register& operator= (Register&&) noexcept = default;
	};

	union RegisterPool
	{
		struct
		{
			Register r0;
			Register r1;
			Register r2;
			Register r3;
			Register r4;
			Register r5;
			Register r6;
			Register r7;
			Register r8;
			Register r9;
			Register r10;

			Register di;
			Register si;
			Register bp;
			Register sp;
			Register ip;
		};
		Register r[16] = {};

	public:
		constexpr RegisterPool() {}
		RegisterPool(const RegisterPool&) = default;
		RegisterPool(RegisterPool&&) noexcept = default;
		~RegisterPool() = default;

		RegisterPool& operator= (const RegisterPool&) = default;
		RegisterPool& operator= (RegisterPool&&) noexcept = default;
	};
}
