namespace N_Core
{
	namespace N_Symbol
	{
		namespace N_Filters
		{
			struct Global
			{
				template <typename SymbolTy>
				bool operator()(SymbolTy const& symbol) 
				{ 
					return symbol.is_global();
				};
			};
			struct Defined
			{
				template <typename SymbolTy>
				bool operator()(SymbolTy const& symbol) 
				{ 
					return symbol.is_defined();
				};
			};
		}

	}
}