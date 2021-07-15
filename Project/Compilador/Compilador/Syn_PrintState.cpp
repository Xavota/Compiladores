#include "pch.h"
#include "Syn_PrintState.h"

#include "Syn_LogicExpresion.h"

namespace Compilador
{
	Syn_PrintState::Syn_PrintState()
	{
	}
	Syn_PrintState::~Syn_PrintState()
	{
	}
	eRETURN_STATE Syn_PrintState::Update(AnalizadorSintactico* syntactic)
	{
		return OpenParenthesis(syntactic);
	}
	eRETURN_STATE Syn_PrintState::OpenParenthesis(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();
		if (tok.GetLexeme() == "(")
		{
			eRETURN_STATE r = Inside(syntactic);
			if (r == eRETURN_STATE::GOOD)
			{
				return Semicolon(syntactic);
			}
			else 
			{
				return r;
			}
		}
		else
		{
			std::string errorMsg = "Expected '(' after print key word on line ";
			errorMsg.append(to_string(tok.GetLine()));
			if (!syntactic->AddError(errorMsg))
			{
				return eRETURN_STATE::FATAL;
			}

			while (tok.GetLexeme() == "(" && tok.GetLexeme() == ")" && tok.GetLexeme() == "}" && tok.GetLexeme() == ";")
			{
				tok = syntactic->GetNextToken();
			}
			if (tok.GetLexeme() == "(")
			{
				return Inside(syntactic);
			}
			else if (tok.GetLexeme() == ")")
			{
				return eRETURN_STATE::GOOD;
			}
			else if (tok.GetLexeme() == "}" || tok.GetLexeme() == ";")
			{
				return eRETURN_STATE::BAD;
			}
			return eRETURN_STATE::BAD;
		}

		return eRETURN_STATE::BAD;
	}
	eRETURN_STATE Syn_PrintState::Inside(AnalizadorSintactico* syntactic)
	{
		Token tok;
		SyntaxState* state = new Syn_LogicExpresion();
		while (true)
		{
			eRETURN_STATE r = state->Update(syntactic);
			if (r == eRETURN_STATE::FATAL)
			{
				return eRETURN_STATE::FATAL;
			}
			else if (r == eRETURN_STATE::BAD)
			{
				tok = syntactic->GetNextToken();
				if (tok.GetLexeme() == "}")
				{
					return eRETURN_STATE::BAD;
				}
				else if (tok.GetLexeme() == ";" || tok.GetLexeme() == ")")
				{
					return eRETURN_STATE::GOOD;
				}
				return eRETURN_STATE::BAD;
			}

			tok = syntactic->GetNextToken();

			if (tok.GetLexeme() == ",")
			{
				continue;
			}
			else if (tok.GetLexeme() == ")")
			{
				return eRETURN_STATE::GOOD;
			}
			else
			{
				std::string errorMsg = "Expected ')' at the end of print call on line ";
				errorMsg.append(to_string(tok.GetLine()));
				if (!syntactic->AddError(errorMsg))
				{
					return eRETURN_STATE::FATAL;
				}

				while (tok.GetLexeme() == ")" && tok.GetLexeme() == "}" && tok.GetLexeme() == ";")
				{
					tok = syntactic->GetNextToken();
				}
				if (tok.GetLexeme() == ")" || tok.GetLexeme() == ";")
				{
					return eRETURN_STATE::GOOD;
				}
				else if (tok.GetLexeme() == "}")
				{
					return eRETURN_STATE::BAD;
				}
				return eRETURN_STATE::BAD;
			}
		}
		return eRETURN_STATE::BAD;
	}
	eRETURN_STATE Syn_PrintState::Semicolon(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();
		if (tok.GetLexeme() == ";")
		{
			return eRETURN_STATE::GOOD;
		}
		else
		{
			std::string errorMsg = "Expected ';' at the end of print call on line ";
			errorMsg.append(to_string(tok.GetLine()));
			if (!syntactic->AddError(errorMsg))
			{
				return eRETURN_STATE::FATAL;
			}

			while (tok.GetLexeme() == "}" && tok.GetLexeme() == ";")
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
			return eRETURN_STATE::BAD;
		}
		return eRETURN_STATE::BAD;
	}
}