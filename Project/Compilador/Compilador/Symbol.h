#pragma once
#include <string>
#include "Token.h"

namespace Compilador
{
	class Symbol
	{
	public:
		Symbol() = default;
		Symbol(std::string name, std::string cathegory, int dimension, std::string functionName, std::string type);
		~Symbol();

		bool SymbolExist(std::string name, std::string cathegory, std::string functionName);
		void AddNextSymbol(std::string name, std::string cathegory, int dimension, std::string functionName, std::string type);

		void UpdateSymboltype(std::string name, std::string cathegory, std::string functionName, std::string newType);

	private:
		std::string m_name;
		std::string m_cathegory;
		int m_dimension = 0;
		std::string m_functionName;
		std::string m_type;

		Symbol* m_next = nullptr;
	};
}
