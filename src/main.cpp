#include <iostream>
#include "src/include/core/symtab/symbol.h"
#include "src/include/core/elf.h"
#include "src/include/core/section/section.h"
int main()
{
	std::vector<N_Core::Elf> SomeVector;
	SomeVector.push_back(N_Core::create_elf("testfiles/sleep"));
}