#include <iostream>
#include "src/include/core/section/section.h"
#include "src/include/core/symtab/symbol.h"
int main()
{
	N_Core::BinaryBlob b;
	N_Core::N_Section::Section component(b, b);

	N_Core::N_SymTab::Symbol symbol(b);
}