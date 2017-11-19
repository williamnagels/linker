#include <iostream>
#include "src/include/core/section/section.h"
#include "src/include/core/symtab/symbol.h"
#include "src/include/core/elf.h"
int main()
{
	std::vector<Linkable> SomeVector;
	SomeVector.push_back(N_Core::create_elf("testfiles/sleep"));
}