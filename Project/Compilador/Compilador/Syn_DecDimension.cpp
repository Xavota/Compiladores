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
			eRETURN_STATE r = eRETURN_STATE::GOOD;
			*m_dim = StrToInt(tok, syntactic, r);
			if (r == eRETURN_STATE::FATAL)
			{
				return eRETURN_STATE::FATAL;
			}
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
			while (tok.GetLexeme() != ")" && tok.GetLexeme() != ";" && tok.GetLexeme() != "}" 
			&& tok.GetLexeme() != "]" && tok.GetType() != eTOKEN_TYPE::END)
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

			while (tok.GetLexeme() != ")" && tok.GetLexeme() != ";" && tok.GetLexeme() != "}" 
			&& tok.GetLexeme() != "]" && tok.GetType() != eTOKEN_TYPE::END)
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
	int Syn_DecDimension::StrToInt(Token t, AnalizadorSintactico* syntactic, eRETURN_STATE& r)
	{
		int res = 0;
		for (char c : t.GetLexeme())
		{
			res *= 10;
			res += c - '0';

			if (res > 65535)
			{
				std::string errorMsg = "Size of variable dimension can't be more than 65535, on line ";
				errorMsg.append(to_string(t.GetLine()));
				if (!syntactic->AddError(errorMsg))
				{
					r = eRETURN_STATE::FATAL;
				}
				return 65535;
			}
		}
		return res;
	}
}