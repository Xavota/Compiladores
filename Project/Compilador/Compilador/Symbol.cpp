#include "pch.h"
#include "Symbol.h"

namespace Compilador
{
	Symbol::Symbol(std::string name, std::string cathegory, int dimension, std::string functionName, std::string type)
	{
		m_name = name;
		m_cathegory = cathegory;
		m_dimension = dimension;
		m_functionName = functionName;
		m_type = type;

		m_next = nullptr;
	}

	Symbol::~Symbol()
	{
		if (m_next != nullptr)
		{
			delete m_next;
			m_next = nullptr;
		}
	}

	bool Symbol::SymbolExist(std::string name, std::string cathegory, std::string functionName)
	{
		if (m_name == name && ((m_cathegory == "FUNCTION" || cathegory == "FUNCTION") || functionName == m_functionName))
		{
			return true;
		}
		else if (m_next != nullptr)
		{
			return m_next->SymbolExist(name, cathegory, functionName);
		}

		return false;
	}

	void Symbol::AddNextSymbol(std::string name, std::string cathegory, int dimension, std::string functionName, std::string type)
	{
		if (m_next != nullptr)
		{
			m_next->AddNextSymbol(name, cathegory, dimension, functionName, type);
		}
		else
		{
			m_next = new Symbol(name, cathegory, dimension, functionName, type);
		}
	}

	void Symbol::UpdateSymboltype(std::string name, std::string cathegory, std::string functionName, std::string newType)
	{
		if (m_name == name && m_cathegory == cathegory && m_functionName == functionName)
		{
			m_type = newType;
		}
		else if (m_next != nullptr)
		{
			m_next->UpdateSymboltype(name, cathegory, functionName, newType);
		}
	}
}