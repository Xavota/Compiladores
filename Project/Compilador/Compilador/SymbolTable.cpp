#include "pch.h"
#include "SymbolTable.h"

namespace Compilador
{
	SymbolTable::SymbolTable()
	{
	}

	SymbolTable::~SymbolTable()
	{
		Clear(); 
	}

	bool SymbolTable::SymbolExist(std::string name, std::string cathegory, std::string functionName)
	{
		if (m_symbols.find(name) != m_symbols.end())
		{
			return m_symbols[name]->SymbolExist(name, cathegory, functionName);
		}
			
		return false;
	}

	void SymbolTable::AddSymbol(int line, std::string name, std::string cathegory, int dimension, std::string functionName, std::string type, int extraInfo)
	{
		if (m_symbols.find(name) != m_symbols.end())
		{
			m_symbols[name]->AddNextSymbol(line, name, cathegory, dimension, functionName, type, extraInfo);
		}
		else
		{
			m_symbols.insert(make_pair(name, new Symbol(line, name, cathegory, dimension, functionName, type, extraInfo)));
		}
	}

	void SymbolTable::UpdateSymboltype(std::string name, std::string cathegory, std::string functionName, std::string newType)
	{
		if (m_symbols.find(name) != m_symbols.end())
		{
			m_symbols[name]->UpdateSymboltype(name, cathegory, functionName, newType);
		}
	}
	std::string SymbolTable::GetType(std::string name, std::string functionName, bool& isFunction)
	{
		if (m_symbols.find(name) != m_symbols.end())
		{
			return m_symbols[name]->GetType(name, functionName, isFunction);
		}
		else
		{
			return "";
		}
	}
	std::map<int, std::string> SymbolTable::GetFuncParameterTypes(std::string functionName)
	{
		std::map<int, std::string> r;
		for (std::map<std::string, Symbol*>::iterator it = m_symbols.begin(); it != m_symbols.end(); it++)
		{
			std::map<int, std::string> rr = it->second->GetFuncParameterTypes(functionName);
			for (std::map<int, std::string>::iterator iti = rr.begin(); iti != rr.end(); iti++)
			{
				r.insert(*iti);
			}
		}
		return r;
	}
	void SymbolTable::Clear()
	{
		for (std::map<std::string, Symbol*>::iterator it = m_symbols.begin(); it != m_symbols.end(); it++)
		{
			delete it->second;
		}
		m_symbols.clear();
	}
}