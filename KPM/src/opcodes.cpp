#include "opcodes.h"

namespace kpm::opcodes
{
	OpcodeManager::OpcodeManager(const OpcodeManager& right) :
		_ops(right._ops.size())
	{
		if (!_ops.empty())
		{
			Opcode* const* src = right._ops.data();
			Opcode** dst = _ops.data();
		}
	}
	OpcodeManager::OpcodeManager(OpcodeManager&& right) noexcept;
	OpcodeManager::~OpcodeManager();

	OpcodeManager& OpcodeManager::operator= (const OpcodeManager& right);
	OpcodeManager& OpcodeManager::operator= (OpcodeManager&& right) noexcept;

	Opcode* read_opcode(void* dst, offset_t dst_off, const void* src, offset_t src_off)
	{
		#define read_concrete_opcode(_Opcode) case _Opcode : \
			reinterpret_cast<opcodes::_Opcode*>(reinterpret_cast<u8*>(dst) + dst_off) \
					->read(reinterpret_cast<const u8*>(src) + src_off); break

		OpcodeIdentifier id;
		utils::raw_read(id, src, src_off);

		switch (id)
		{
			using enum OpcodeIdentifier;

			read_concrete_opcode(move_r_r);
			read_concrete_opcode(move_r_a);
		}

		return reinterpret_cast<Opcode*>(reinterpret_cast<u8*>(dst) + dst_off);
		#undef read_concrete_opcode
	}

	constexpr const char* get_register_name(args::RegisterId reg)
	{
		switch (reg)
		{
			using enum args::RegisterId;
			case r0: return "r0";
			case r1: return "r1";
			case r2: return "r2";
			case r3: return "r3";
			case r4: return "r4";
			case r5: return "r5";
			case r6: return "r6";
			case r7: return "r7";
			case r8: return "r8";
			case r9: return "r9";
			case r10: return "r10";
			case di: return "di";
			case si: return "si";
			case bp: return "bp";
			case sp: return "sp";
			case ip: return "ip";
			default: return "<invalid-register>";
		}
	}

	constexpr std::string operator+ (const std::string& left, args::RegisterId right)
	{
		return left + get_register_name(right);
	}

	constexpr std::string operator+ (args::RegisterId left, const std::string& right)
	{
		return get_register_name(left) + right;
	}

	constexpr std::string str(const char* string) { return std::string(string); }

	constexpr std::string str(args::TwoRegisters value) { return value.target + " " + value.source; }

	constexpr const char* str_scale(args::SizeScaleIndex ssi)
	{
		switch (ssi.scale)
		{
			case 0b00: return "";
			case 0b01: return "*2";
			case 0b10: return "*4";
			case 0b11: return "*8";
			default: return "<invalid-scale>";
		}
	}

	constexpr const char* str_size(args::DataSize size)
	{
		switch (size)
		{
			using enum args::DataSize;
			case bt: return "bt";
			case wd: return "wd";
			case dw: return "dw";
			case qw: return "qw";
			default: return "<invalid-size>";
		}
	}

	template <bool _IsSrc>
	constexpr std::string str(args::SizeScaleIndex ssi, args::TwoRegisters regs, args::ImmediateValue displ)
	{
		std::string str;
		if constexpr (_IsSrc)
			str = "[" + regs.source + "+" + ssi.index + str_scale(ssi);
		else
			str = "[" + regs.target + "+" + ssi.index + str_scale(ssi);
		if (ssi.displacement)
			str += opcodes::str("+") + std::to_string(displ.qword) + "]";
		else
			str += "]";
		return str;
	}



	void move_r_r::read(const void* buff)
	{
		_id = OpcodeIdentifier::move_r_r;
		utils::raw_read(size, buff, 1);
		utils::raw_read(regs, buff, 2);
	}

	void move_r_r::write(void* buff) const
	{
		utils::raw_write(buff, _id);
		utils::raw_write(buff, size, 1);
		utils::raw_write(buff, regs, 2);
	}

	std::string move_r_r::to_string()
	{
		return "move_r_r " + str(str_size(size)) + " " + str(regs);
	}


	void move_r_a::read(const void* buff)
	{
		_id = OpcodeIdentifier::move_r_a;
		utils::raw_read(size, buff, 1);
		utils::raw_read(ssi, buff, 2);
		utils::raw_read(regs, buff, 3);
		if (ssi.displacement)
			utils::raw_read(displ, buff, 4);
	}

	void move_r_a::write(void* buff) const
	{
		utils::raw_write(buff, _id);
		utils::raw_write(buff, size);
		utils::raw_write(buff, ssi, 2);
		utils::raw_write(buff, regs, 3);
		if (ssi.displacement)
			utils::raw_write(buff, displ, 4);
	}

	std::string move_r_a::to_string()
	{
		return "move_r_a " + str(str_size(size)) + " " + str<false>(ssi, regs, displ);
	}
}
