#pragma once

namespace N_Core
{
	namespace N_Symbol
	{
		namespace N_Filters
		{
			struct HasName
			{
				std::string _name;
				template <typename SymbolTy>
				bool operator()(SymbolTy const& symbol) 
				{ 
					auto opt_name = symbol.get_name_as_string();

					if (opt_name && *opt_name == _name)
					{
						return true;
					}
					return false;
				};
			};
			struct HasIndex
			{
				N_Core::Index _index;
				template <typename SymbolTy>
				bool operator()(SymbolTy const& symbol) 
				{ 
					return _index == symbol.get_section_index();
				};
			};
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