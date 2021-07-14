#include "pch.h"
#include "Syn_ReturnState.h"
#include "Syn_LogicExpresion.h"

namespace Compilador
{
	Syn_ReturnState::Syn_ReturnState()
	{
	}
	Syn_ReturnState::~Syn_ReturnState()
	{
	}
	eRETURN_STATE Syn_ReturnState::Update(AnalizadorSintactico* syntactic)
	{
		SyntaxState* state = new Syn_LogicExpresion();
		eRETURN_STATE r = state->Update(syntactic);
		delete state;
		if (r == eRETURN_STATE::GOOD)
		{
			Token tok = syntactic->GetNextToken();
			if (tok.GetLexeme() == ";")
			{
				return eRETURN_STATE::GOOD;
			}
			else
			{
				std::string errorMsg = "Expected ';' at the end of return statement on line ";
				errorMsg.append(to_string(tok.GetLine()));
				if (!syntactic->AddError(errorMsg))
				{
					return eRETURN_STATE::FATAL;
				}

				// Panik mode
				while (tok.GetLexeme() != ";" && tok.GetLexeme() != "}")
				{
					tok = syntactic->GetNextToken();
				}
				if (tok.GetLexeme() == "}")
				{
					return eRETURN_STATE::BAD;
				}
				else if (tok.GetLexeme() == ";")
				{
					return eRETURN_STATE::GOOD;
				}
				return eRETURN_STATE::BAD;
			}
		}
		else if (r == eRETURN_STATE::BAD)
		{
			Token tok = syntactic->GetNextToken();
			if (tok.GetLexeme() == "{")
			{
				return eRETURN_STATE::BAD;
			}
			else if (tok.GetLexeme() == ";")
			{
				return eRETURN_STATE::GOOD;
			}
			return eRETURN_STATE::BAD;
		}
		else if (r == eRETURN_STATE::FATAL)
		{
			return eRETURN_STATE::FATAL;
		}

		return eRETURN_STATE();
	}
}