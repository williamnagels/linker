namespace N_Core
{
	namespace N_Filters
	{
		namespace N_Symbol
		{
			template <typename ElfTy>
			struct Global
			{
				bool operator()(typename ElfTy::SectionTy::SymbolTableTy::SymbolTy const& symbol) 
				{ 
					return symbol.is_global();
				};
			};
		}

	}
}