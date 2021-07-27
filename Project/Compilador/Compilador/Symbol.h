#pragma once
#include <string>
#include <map>
#include "Token.h"

namespace Compilador
{
	class Symbol
	{
	public:
		Symbol() = default;
		Symbol(int line, std::string name, std::string cathegory, int dimension, std::string functionName, std::string type, int extraInfo);
		~Symbol();

		bool SymbolExist(std::string name, std::string cathegory, std::string functionName);
		void AddNextSymbol(int line, std::string name, std::string cathegory, int dimension, std::string functionName, std::string type, int extraInfo);

		void UpdateSymboltype(std::string name, std::string cathegory, std::string functionName, std::string newType);
		
		std::string GetType(std::string name, std::string functionName, bool& isFunction);

		std::map<int,std::string> GetFuncParameterTypes(std::string functionName);

	public:
		int m_line;
		std::string m_name;
		std::string m_cathegory;
		int m_dimension = 0;
		std::string m_functionName;
		std::string m_type;

		int m_extraInfo = 0;

		Symbol* m_next = nullptr;
	};
}
