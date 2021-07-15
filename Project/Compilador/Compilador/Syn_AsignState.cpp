#include "pch.h"
#include "Syn_AsignState.h"

#include "Syn_LogicExpresion.h"

namespace Compilador
{
	Syn_AsignState::Syn_AsignState()
	{
	}
	Syn_AsignState::~Syn_AsignState()
	{
	}
	eRETURN_STATE Syn_AsignState::Update(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();

		return Equal(syntactic);
	}
	eRETURN_STATE Syn_AsignState::Equal(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();
		if (tok.GetLexeme() == "=")
		{
			SyntaxState* state = new Syn_LogicExpresion();
			eRETURN_STATE r = state->Update(syntactic);
			delete state;
			if (r == eRETURN_STATE::GOOD)
			{
				return SemiColon(syntactic);
			}
			else if (r == eRETURN_STATE::BAD)
			{
				tok = syntactic->GetNextToken();
				if (tok.GetLexeme() == ";")
				{
					return eRETURN_STATE::GOOD;
				}
				
				return eRETURN_STATE::BAD;
			}
			else if (r == eRETURN_STATE::FATAL)
			{
				return eRETURN_STATE::FATAL;
			}
		}
		else
		{
			std::string errorMsg = "Expected '=' after ID on asignment on line ";
			errorMsg.append(to_string(tok.GetLine()));

			//Panik mode
			while (tok.GetLexeme() != ";" && tok.GetLexeme() != "}" && tok.GetLexeme() == "=")
			{
				tok = syntactic->GetNextToken();
			}
			if (tok.GetLexeme() == "=")
			{
				SyntaxState* state = new Syn_LogicExpresion();
				eRETURN_STATE r = state->Update(syntactic);
				delete state;
				if (r == eRETURN_STATE::GOOD)
				{
					SemiColon(syntactic);
				}
				else if (r == eRETURN_STATE::BAD)
				{
					tok = syntactic->GetNextToken();
					if (tok.GetLexeme() == ";")
					{
						return eRETURN_STATE::GOOD;
					}
				}
				else if (r == eRETURN_STATE::FATAL)
				{
					return eRETURN_STATE::FATAL;
				}
				return eRETURN_STATE::BAD;
			}
			else if (tok.GetLexeme() == ";")
			{
				return eRETURN_STATE::GOOD;
			}
			else if (tok.GetLexeme() == "}")
			{
				return eRETURN_STATE::BAD;
			}
			return eRETURN_STATE::BAD;
		}
		return eRETURN_STATE::BAD;
	}
	eRETURN_STATE Syn_AsignState::SemiColon(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();
		if (tok.GetLexeme() == ";")
		{
			return eRETURN_STATE::GOOD;
		}
		else
		{
			std::string errorMsg = "Expected ';' after logic expresion on asignment on line ";
			errorMsg.append(to_string(tok.GetLine()));

			//Panik mode
			while (tok.GetLexeme() != ";" && tok.GetLexeme() != "}")
			{
				tok = syntactic->GetNextToken();
			}
			if (tok.GetLexeme() == ";")
			{
				return eRETURN_STATE::GOOD;
			}
			else if (tok.GetLexeme() == "}")
			{
				return eRETURN_STATE::BAD;
			}
		}
		return eRETURN_STATE::BAD;
	}
}
