#include "pch.h"
#include "Syn_MainState.h"

#include "Syn_FunctionBlock.h"

namespace Compilador
{
	Syn_MainState::Syn_MainState()
	{
	}
	Syn_MainState::~Syn_MainState()
	{
	}
	eRETURN_STATE Syn_MainState::Update(AnalizadorSintactico* syntactic)
	{		
		return OpenParenthesis(syntactic);
	}
	eRETURN_STATE Syn_MainState::OpenParenthesis(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();

		if (tok.GetLexeme() == "(")
		{
			return CloseParenthesis(syntactic);
		}
		else
		{
			std::string errorMsg = "Expected '(' after 'main' on line ";
			errorMsg.append(to_string(tok.GetLine()));
			if (!syntactic->AddError(errorMsg))
			{
				syntactic->SetContext("");
				return eRETURN_STATE::FATAL;
			}

			// Panik mode
			while (tok.GetLexeme() != "{" && tok.GetLexeme() != "}" && tok.GetLexeme() != "(" 
			&& tok.GetLexeme() != ")" && tok.GetType() != eTOKEN_TYPE::END)
			{
				tok = syntactic->GetNextToken();
			}
			if (tok.GetLexeme() == "(")
			{
				return CloseParenthesis(syntactic);
			}
			else if (tok.GetLexeme() == ")")
			{
				return OpenBrackets(syntactic);
			}
			else if (tok.GetLexeme() == "{")
			{
				SyntaxState* state = new Syn_FunctionBlock(&m_hasReturn);
				eRETURN_STATE r = state->Update(syntactic);
				delete state;
				syntactic->SetContext("");
				return r;
			}
			else if (tok.GetLexeme() == "}")
			{
				syntactic->SetContext("");
				return eRETURN_STATE::GOOD;
			}
			else if (tok.GetType() == eTOKEN_TYPE::END)
			{
				syntactic->SetContext("");
				return eRETURN_STATE::BAD;
			}
			syntactic->SetContext("");
			return eRETURN_STATE::BAD;
		}
		syntactic->SetContext("");
		return eRETURN_STATE::BAD;
	}
	eRETURN_STATE Syn_MainState::CloseParenthesis(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();
		if (tok.GetLexeme() == ")")
		{
			return OpenBrackets(syntactic);
		}
		else
		{
			std::string errorMsg = "Expected ')' after '(' on Main declaration on line ";
			errorMsg.append(to_string(tok.GetLine()));
			if (!syntactic->AddError(errorMsg))
			{
				syntactic->SetContext("");
				return eRETURN_STATE::FATAL;
			}

			// Panik mode
			while (tok.GetLexeme() != "{" && tok.GetLexeme() != "}" && tok.GetLexeme() != ")"
				&& tok.GetType() != eTOKEN_TYPE::END)
			{
				tok = syntactic->GetNextToken();
			}
			if (tok.GetLexeme() == ")")
			{
				return OpenBrackets(syntactic);
			}
			else if (tok.GetLexeme() == "{")
			{
				SyntaxState* state = new Syn_FunctionBlock(&m_hasReturn);
				eRETURN_STATE r = state->Update(syntactic);
				delete state;
				syntactic->SetContext("");
				return r;
			}
			else if (tok.GetLexeme() == "}")
			{
				syntactic->SetContext("");
				return eRETURN_STATE::GOOD;
			}
			else if (tok.GetType() == eTOKEN_TYPE::END)
			{
				syntactic->SetContext("");
				return eRETURN_STATE::BAD;
			}
			syntactic->SetContext("");
			return eRETURN_STATE::BAD;
		}
		syntactic->SetContext("");
		return eRETURN_STATE::BAD;
	}
	eRETURN_STATE Syn_MainState::OpenBrackets(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();
		if (tok.GetLexeme() == "{")
		{
			SyntaxState* state = new Syn_FunctionBlock(&m_hasReturn);
			eRETURN_STATE r = state->Update(syntactic);
			delete state;
			syntactic->SetContext("");
			return r;
		}
		else
		{
			std::string errorMsg = "Expected '{' after Main declaration on line ";
			errorMsg.append(to_string(tok.GetLine()));
			if (!syntactic->AddError(errorMsg))
			{
				syntactic->SetContext("");
				return eRETURN_STATE::FATAL;
			}

			// Panik mode
			while (tok.GetLexeme() != "}" && tok.GetType() != eTOKEN_TYPE::END)
			{
				tok = syntactic->GetNextToken();
			}
			if (tok.GetLexeme() == "}")
			{
				syntactic->SetContext("");
				return eRETURN_STATE::GOOD;
			}
			else if (tok.GetType() == eTOKEN_TYPE::END)
			{
				syntactic->SetContext("");
				return eRETURN_STATE::BAD;
			}
			syntactic->SetContext("");
			return eRETURN_STATE::BAD;
		}
		syntactic->SetContext("");
		return eRETURN_STATE::BAD;
	}
}
