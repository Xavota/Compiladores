#include "pch.h"
#include "SymbolTable.h"

namespace Compilador
{
	SymbolTable::SymbolTable()
	{
	}

	SymbolTable::~SymbolTable()
	{
		m_symbols.clear();
	}

	bool SymbolTable::SymbolExist(std::string name, std::string cathegory, std::string functionName)
	{
		if (m_symbols.find(name) != m_symbols.end())
		{
			return m_symbols[name]->SymbolExist(name, cathegory, functionName);
		}
			
		return false;
	}

	void SymbolTable::AddSymbol(int line, std::string name, std::string cathegory, int dimension, std::string functionName, std::string type)
	{
		if (m_symbols.find(name) != m_symbols.end())
		{
			m_symbols[name]->AddNextSymbol(line, name, cathegory, dimension, functionName, type);
		}
		else
		{
			m_symbols.insert(make_pair(name, new Symbol(line, name, cathegory, dimension, functionName, type)));
		}
	}

	void SymbolTable::UpdateSymboltype(std::string name, std::string cathegory, std::string functionName, std::string newType)
	{
		if (m_symbols.find(name) != m_symbols.end())
		{
			m_symbols[name].UpdateSymboltype(name, cathegory, functionName, newType);
		}
	}
}