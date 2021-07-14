#include "pch.h"
#include "Syn_DecDimension.h"

namespace Compilador
{
	Syn_DecDimension::Syn_DecDimension(int* dim)
	{
		m_dim = dim;
	}
	Syn_DecDimension::~Syn_DecDimension()
	{
	}
	eRETURN_STATE Syn_DecDimension::Update(AnalizadorSintactico* syntactic)
	{		
		return GetInt(syntactic);
	}
	eRETURN_STATE Syn_DecDimension::GetInt(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();
		if (tok.GetType() == eTOKEN_TYPE::INT_CONST)
		{
			*m_dim = atoi(tok.GetLexeme().c_str());
			return CloseSquare(syntactic);
		}
		else
		{
			std::string errorMsg = "Expected an int constant on declaration dimension on line ";
			errorMsg.append(to_string(tok.GetLine()));
			if (!syntactic->AddError(errorMsg))
			{
				return  eRETURN_STATE::FATAL;
			}			

			//Panik mode
			while (tok.GetLexeme() != ")" && tok.GetLexeme() != ";" && tok.GetLexeme() != "}" && tok.GetLexeme() != "]")
			{
				tok = syntactic->GetNextToken();
			}
			if (tok.GetLexeme() == "]")
			{
				return eRETURN_STATE::GOOD;
			}
			return eRETURN_STATE::BAD;
		}
		return eRETURN_STATE::BAD;
	}
	eRETURN_STATE Syn_DecDimension::CloseSquare(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();
		if (tok.GetLexeme() == "]")
		{
			return eRETURN_STATE::GOOD;
		}
		else
		{
			std::string errorMsg = "Expected ']' at declaration dimension on line ";
			errorMsg.append(to_string(tok.GetLine()));
			if (!syntactic->AddError(errorMsg))
			{
				return eRETURN_STATE::FATAL;
			}

			while (tok.GetLexeme() != ")" && tok.GetLexeme() != ";" && tok.GetLexeme() != "}" && tok.GetLexeme() != "]")
			{
				tok = syntactic->GetNextToken();
			}
			if (tok.GetLexeme() == "]")
			{
				return eRETURN_STATE::GOOD;
			}
			return eRETURN_STATE::BAD;
		}
		return eRETURN_STATE::BAD;
	}
}