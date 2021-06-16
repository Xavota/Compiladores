#pragma once
#include "Symbol.h"
#include <vector>
#include <map>

namespace Compilador
{
	class SymbolTable
	{
	public:
		SymbolTable();
		~SymbolTable();

		bool SymbolExist(std::string name, std::string cathegory, std::string functionName);
		void AddSymbol(std::string name, std::string cathegory, int dimension, std::string functionName, std::string type);

		void UpdateSymboltype(std::string name, std::string cathegory, std::string functionName, std::string newType);

	private:
		std::map<std::string, Symbol> m_symbols;
	};
}
