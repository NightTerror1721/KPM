module;

#include "common.h"

export module registers;

export namespace kpm
{
	union Register
	{
		BYTE b;
		WORD w;
		LONG l;
		QUAD q = 0;
		struct
		{
			bool CF : 1; //0
			bool : 1; //1
			bool PF : 1; //2
			bool : 1; //3
			bool AF : 1; //4
			bool : 1; //5
			bool ZF : 1; //6
			bool SF : 1; //7
			bool TF : 1; //8
			bool IF : 1; //9
			bool DF : 1; //10
			bool OF : 1; //11
			bool : 4; //12-15
			bool RF : 1; //16
		} f;

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

			Register fr;
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