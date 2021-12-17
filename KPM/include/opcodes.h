#pragma once

#include "common.h"

namespace kpm::opcodes
{
	enum class OpcodeIdentifier : u8
	{
		move_r_r,
		move_r_a,
		move_r_i,
		move_a_r,
	};

	namespace args
	{
		enum class RegisterId : u8
		{
			r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10,
			di, si, bp, sp, ip
		};

		enum class DataSize : u8 { bt, wd, dw, qw };

		struct TwoRegisters
		{
			RegisterId target : 4;
			RegisterId source : 4;
		};

		struct SizeScaleIndex
		{
			DataSize size : 2;
			u8 scale : 2;
			RegisterId index : 4;
		};
	}



	class Opcode
	{
	private:
		OpcodeIdentifier _id;

	public:
		inline Opcode(OpcodeIdentifier id) : _id{ id } {}

		inline OpcodeIdentifier id() const { return _id; }

	public:
		virtual std::string to_string() = 0;
		virtual size_t size_in_bytes() const = 0;
		virtual void read(const void* buff) = 0;
		virtual void write(void* buff) const = 0;
	};

	struct move_r_r : public Opcode
	{
		args::SizeScaleIndex ssi;
		args::TwoRegisters regs;

		inline move_r_r(args::SizeScaleIndex ssi, args::TwoRegisters registers) :
			Opcode(OpcodeIdentifier::move_r_r), ssi(ssi), regs(registers) {}
		inline move_r_r() : Opcode(OpcodeIdentifier::move_r_r), ssi(), regs() {}

		inline size_t size_in_bytes() const override { return 3; }

		void read(const void* buff) override;
		void write(void* buff) const override;
		std::string to_string() override;
	};

	struct move_r_a : public Opcode
	{
		args::SizeScaleIndex ssi;
		args::TwoRegisters regs;

		inline move_r_a(args::SizeScaleIndex ssi, args::TwoRegisters registers) :
			Opcode(OpcodeIdentifier::move_r_a), ssi(ssi), regs(registers) {}
		inline move_r_a() : Opcode(OpcodeIdentifier::move_r_r), ssi(), regs() {}

		inline size_t size_in_bytes() const override { return 3; }

		void read(const void* buff) override;
		void write(void* buff) const override;
		std::string to_string() override;
	};
}
