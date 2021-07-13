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
		void AddSymbol(int line, std::string name, std::string cathegory, int dimension, std::string functionName, std::string type);

		void UpdateSymboltype(std::string name, std::string cathegory, std::string functionName, std::string newType);

	public:
		std::map<std::string, Symbol*> m_symbols;
	};
}
