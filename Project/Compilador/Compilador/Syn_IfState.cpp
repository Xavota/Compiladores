#include "pch.h"
#include "Syn_IfState.h"
#include "Syn_Statements.h"
#include "Syn_LogicExpresion.h"

namespace Compilador
{
	Syn_IfState::Syn_IfState(bool* hasReturn)
	{
		m_hasReturn = hasReturn;
	}
	Syn_IfState::~Syn_IfState()
	{
	}
	eRETURN_STATE Syn_IfState::Update(AnalizadorSintactico* syntactic)
	{
		return OpenParenthesis(syntactic);
	}
	eRETURN_STATE Syn_IfState::OpenParenthesis(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();
		if (tok.GetLexeme() == "(")
		{
			SyntaxState* state = new Syn_LogicExpresion();
			eRETURN_STATE r = state->Update(syntactic);
			delete state;
			if (r == eRETURN_STATE::GOOD)
			{
				return CloseParenthesis(syntactic);
			}
			else if (r == eRETURN_STATE::BAD)
			{
				tok = syntactic->GetNextToken();
				if (tok.GetLexeme() == "(")
				{
					return eRETURN_STATE::BAD;
				}
				else if (tok.GetLexeme() == ")")
				{
					return OpenBrackets(syntactic);
				}
				else if (tok.GetLexeme() == "{")
				{
					SyntaxState* state = new Syn_Statements(m_hasReturn);
					r = state->Update(syntactic);
					delete state;
					if (r == eRETURN_STATE::GOOD)
					{
						return CloseBrackets(syntactic);
					}
					else
					{
						return r;
					}
				}
				else if (tok.GetLexeme() == "}")
				{
					return eRETURN_STATE::GOOD;
				}
				return eRETURN_STATE::BAD;
			}
		}
		else
		{
			std::string errorMsg = "Expected '(' after if keyword on line ";
			errorMsg.append(to_string(tok.GetLine()));
			syntactic->AddError(errorMsg);

			//Panik mode
			while (tok.GetLexeme() != "(" && tok.GetLexeme() != ")" && tok.GetLexeme() != "{" && tok.GetLexeme() == "}")
			{
				tok = syntactic->GetNextToken();
			}
			if (tok.GetLexeme() == "(")
			{
				SyntaxState* state = new Syn_LogicExpresion();
				eRETURN_STATE r = state->Update(syntactic);
				delete state;
				if (r == eRETURN_STATE::GOOD)
				{
					return CloseParenthesis(syntactic);
				}
				else if (r == eRETURN_STATE::BAD)
				{
					tok = syntactic->GetNextToken();
					if (tok.GetLexeme() == "(")
					{
						return eRETURN_STATE::BAD;
					}
					else if (tok.GetLexeme() == ")")
					{
						return OpenBrackets(syntactic);
					}
					else if (tok.GetLexeme() == "{")
					{
						SyntaxState* state = new Syn_Statements(m_hasReturn);
						r = state->Update(syntactic);
						delete state;
						if (r == eRETURN_STATE::GOOD)
						{
							return CloseBrackets(syntactic);
						}
						else
						{
							return r;
						}
					}
					else if (tok.GetLexeme() == "}")
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
			else if (tok.GetLexeme() == ")")
			{
				return OpenBrackets(syntactic);
			}
			else if (tok.GetLexeme() == "{")
			{
				SyntaxState* state = new Syn_Statements(m_hasReturn);
				eRETURN_STATE r = state->Update(syntactic);
				delete state;
				if (r == eRETURN_STATE::GOOD)
				{
					return CloseBrackets(syntactic);
				}
				else
				{
					return r;
				}
			}
			else if (tok.GetLexeme() == "}")
			{
				return eRETURN_STATE::GOOD;
			}
		}
		return eRETURN_STATE::BAD;
	}
	eRETURN_STATE Syn_IfState::CloseParenthesis(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();
		if (tok.GetLexeme() == ")")
		{
			return OpenBrackets(syntactic);
		}
		else
		{
			std::string errorMsg = "Expected ')' after logic expresion on line ";
			errorMsg.append(to_string(tok.GetLine()));
			syntactic->AddError(errorMsg);

			//Panik mode
			while (tok.GetLexeme() != ")" && tok.GetLexeme() != "{" && tok.GetLexeme() == "}")
			{
				tok = syntactic->GetNextToken();
			}
			if (tok.GetLexeme() == ")")
			{
				return OpenBrackets(syntactic);
			}
			else if (tok.GetLexeme() == "{")
			{
				SyntaxState* state = new Syn_Statements(m_hasReturn);
				eRETURN_STATE r = state->Update(syntactic);
				delete state;
				if (r == eRETURN_STATE::GOOD)
				{
					return CloseBrackets(syntactic);
				}
				else
				{
					return r;
				}
			}
			else if (tok.GetLexeme() == "}")
			{
				return eRETURN_STATE::GOOD;
			}
		}
		return eRETURN_STATE::BAD;
	}
	eRETURN_STATE Syn_IfState::OpenBrackets(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();
		if (tok.GetLexeme() == "{")
		{
			SyntaxState* state = new Syn_Statements(m_hasReturn);
			eRETURN_STATE r = state->Update(syntactic);
			delete state;
			if (r == eRETURN_STATE::GOOD)
			{
				return CloseBrackets(syntactic);
			}
			else
			{
				return r;
			}
		}
		else
		{
			std::string errorMsg = "Expected '{' before block of statements on line ";
			errorMsg.append(to_string(tok.GetLine()));
			syntactic->AddError(errorMsg);

			//Panik mode
			while (tok.GetLexeme() != "{" && tok.GetLexeme() == "}")
			{
				tok = syntactic->GetNextToken();
			}
			if (tok.GetLexeme() == "{")
			{
				SyntaxState* state = new Syn_Statements(m_hasReturn);
				eRETURN_STATE r = state->Update(syntactic);
				delete state;
				if (r == eRETURN_STATE::GOOD)
				{
					return CloseBrackets(syntactic);
				}
				else
				{
					return r;
				}
			}
			else if (tok.GetLexeme() == "}")
			{
				return eRETURN_STATE::GOOD;
			}
		}
		return eRETURN_STATE::BAD;
	}
	eRETURN_STATE Syn_IfState::CloseBrackets(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();
		if (tok.GetLexeme() == "}")
		{
			return eRETURN_STATE::GOOD;
		}
		else
		{
			std::string errorMsg = "Expected '}' after logic expresion on line ";
			errorMsg.append(to_string(tok.GetLine()));
			syntactic->AddError(errorMsg);

			//Panik mode
			while (tok.GetLexeme() == "}")
			{
				tok = syntactic->GetNextToken();
			}
			if (tok.GetLexeme() == "}")
			{
				return eRETURN_STATE::GOOD;
			}
		}
		return eRETURN_STATE::BAD;
	}
}