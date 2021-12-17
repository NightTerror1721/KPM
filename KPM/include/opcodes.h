#pragma once

#include "common.h"

namespace kpm::opcodes
{
	class Opcode;

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
			bool displacement : 1;
			bool : 1;
			u8 scale : 2;
			RegisterId index : 4;
		};

		union ImmediateValue
		{
			u8 byte;
			u16 word;
			u32 dword;
			u64 qword = 0;

			s8 sbyte;
			s16 sword;
			s32 sdword;
			s64 sqword;
		};
	}

	
	Opcode* read_opcode(void* dst, offset_t dst_off, const void* src, offset_t src_off);



	class Opcode
	{
	protected:
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
		args::DataSize size;
		args::TwoRegisters regs;

		inline move_r_r(args::DataSize size, args::TwoRegisters regs) :
			Opcode(OpcodeIdentifier::move_r_r), size(size), regs(regs) {}
		inline move_r_r() : Opcode(OpcodeIdentifier::move_r_r), size(), regs() {}

		inline size_t size_in_bytes() const override { return 3; }

		void read(const void* buff) override;
		void write(void* buff) const override;
		std::string to_string() override;
	};



	class OpcodeManager
	{
	private:
		std::vector<Opcode*> _ops;

	public:
		OpcodeManager() = default;

		OpcodeManager(const OpcodeManager& right);
		OpcodeManager(OpcodeManager&& right) noexcept;
		~OpcodeManager();

		OpcodeManager& operator= (const OpcodeManager& right);
		OpcodeManager& operator= (OpcodeManager&& right) noexcept;
	};






	struct move_r_a : public Opcode
	{
		args::DataSize size;
		args::SizeScaleIndex ssi;
		args::TwoRegisters regs;
		args::ImmediateValue displ;

		inline move_r_a(
			args::DataSize size,
			args::SizeScaleIndex ssi,
			args::TwoRegisters regs,
			args::ImmediateValue displ = {}) :
			Opcode(OpcodeIdentifier::move_r_a), size(size), ssi(ssi), regs(regs), displ(displ) {}
		inline move_r_a() : Opcode(OpcodeIdentifier::move_r_r), size(), ssi(), regs(), displ() {}

		inline size_t size_in_bytes() const override { return ssi.displacement ? 5 : 4; }

		void read(const void* buff) override;
		void write(void* buff) const override;
		std::string to_string() override;
	};
}
