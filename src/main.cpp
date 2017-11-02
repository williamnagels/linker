#include <iostream>
#include "src/include/core/section/section.h"
#include "src/include/core/symtab/symbol.h"
#include "src/include/core/elf.h"
int main()
{
	N_Core::create_elf_from_path_to_file("testfiles/sleep");
}