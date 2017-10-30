#include <iostream>
#include "src/include/core/section/section.h"
#include "src/include/core/symtab/symbol.h"
#include "src/include/core/elf.h"
int main()
{
	N_Core::BinaryBlob b;
	N_Core::N_Section::Section component(b, b);

	N_Core::N_SymTab::Symbol symbol(b);

	N_Core::create_elf_from_path_to_file("test");
}