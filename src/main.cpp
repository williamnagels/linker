#include <iostream>
#include "src/include/core/symtab/symbol.h"
#include "src/include/core/elf.h"
#include "src/include/core/general.h"
int main()
{
	std::vector<N_Core::Elf<N_Core::Bit64>> SomeVector;
	SomeVector.push_back(N_Core::create_elf<N_Core::Bit64>("testfiles/sleep"));
}