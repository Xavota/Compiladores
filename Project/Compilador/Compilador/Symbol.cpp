#include "pch.h"
#include "Symbol.h"

namespace Compilador
{
	Symbol::Symbol(int line, std::string name, std::string cathegory, int dimension, std::string functionName, std::string type, int extraInfo)
	{
		m_line = line;
		m_name = name;
		m_cathegory = cathegory;
		m_dimension = dimension;
		m_functionName = functionName;
		m_type = type;

		m_extraInfo = extraInfo;

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

	void Symbol::AddNextSymbol(int line, std::string name, std::string cathegory, int dimension, std::string functionName, std::string type, int extraInfo)
	{
		if (m_next != nullptr)
		{
			m_next->AddNextSymbol(line, name, cathegory, dimension, functionName, type, extraInfo);
		}
		else
		{
			m_next = new Symbol(line, name, cathegory, dimension, functionName, type, extraInfo);
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
	std::string Symbol::GetType(std::string name, std::string functionName, bool& isFunction)
	{
		if (m_functionName == functionName)
		{
			return m_type;
		}
		else
		{
			if (m_next != nullptr)
			{
				std::string type = m_next->GetType(name, functionName, isFunction);
				if (type != "")
				{
					return type;
				}
				else
				{
					if (m_cathegory == "FUNCTION")
					{
						isFunction = true;
						return m_type;
					}
					else if (m_cathegory == "GLOBAL")
					{
						return m_type;
					}
					else
					{
						return "";
					}
				}
			}
			else
			{
				if (m_cathegory == "FUNCTION")
				{
					isFunction = true;
					return m_type;
				}
				else if (m_cathegory == "GLOBAL")
				{
					return m_type;
				}
				else
				{
					return "";
				}
			}
		}
	}
	std::map<int, std::string> Symbol::GetFuncParameterTypes(std::string functionName)
	{
		std::map<int, std::string> r;
		if (m_next != nullptr)
		{
			r = m_next->GetFuncParameterTypes(functionName);
		}

		if (m_cathegory == "PARAMETER" && m_functionName == functionName)
		{
			r.insert(std::make_pair(m_extraInfo, m_type));
		}
		return r;
	}
}