#include "common.h"
#include "registers.h"

int main(int argc, char** argv)
{
	kpm::RegisterPool regs;

	regs.r[8].q = 36;

	return 0;
}
