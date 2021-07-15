#include "pch.h"
#include "Syn_ForState.h"

#include "Syn_AsignState.h"
#include "Syn_LogicExpresion.h"
#include "Syn_Statements.h"

namespace Compilador
{
	Syn_ForState::Syn_ForState(bool* hasReturn)
	{
		m_hasReturn = hasReturn;
	}
	Syn_ForState::~Syn_ForState()
	{
	}
	eRETURN_STATE Syn_ForState::Update(AnalizadorSintactico* syntactic)
	{
		return OpenParenthesis(syntactic);
	}
	eRETURN_STATE Syn_ForState::OpenParenthesis(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();
		if (tok.GetLexeme() == "(")
		{
			return VariableAsignation(syntactic);
		}
		else
		{
			std::string errorMsg = "Expected '(' after for key word on line ";
			errorMsg.append(to_string(tok.GetLine()));
			if (!syntactic->AddError(errorMsg))
			{
				return eRETURN_STATE::FATAL;
			}

			//Panik Mode
			while (tok.GetLexeme() != "(" && tok.GetLexeme() != ")" && tok.GetLexeme() != "{" && tok.GetLexeme() != "}")
			{
				tok = syntactic->GetNextToken();
			}
			if (tok.GetLexeme() == "(")
			{
				return VariableAsignation(syntactic);
			}
			else if (tok.GetLexeme() == ")")
			{
				return OpenBrackets(syntactic);
			}
			else if (tok.GetLexeme() == "{")
			{
				SyntaxState* state = new Syn_Statements(m_hasReturn);
				eRETURN_STATE r = state->Update(syntactic);
				if (r == eRETURN_STATE::GOOD)
				{
					return CloseBrackets(syntactic);
				}
				else if (r == eRETURN_STATE::BAD)
				{
					syntactic->Putback(1);
					tok = syntactic->GetNextToken();
					if (tok.GetLexeme() == "}")
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
			else if (tok.GetLexeme() == "}")
			{
				return eRETURN_STATE::GOOD;
			}
		}
		return eRETURN_STATE::BAD;
	}
	eRETURN_STATE Syn_ForState::VariableAsignation(AnalizadorSintactico* syntactic)
	{
		SyntaxState* state = new Syn_AsignState();
		eRETURN_STATE r = state->Update(syntactic);
		if (r == eRETURN_STATE::GOOD)
		{
			return Condition(syntactic);
		}
		else if (r == eRETURN_STATE::BAD)
		{
			syntactic->Putback(1);
			Token tok = syntactic->GetNextToken();
			if (tok.GetLexeme() == ")")
			{
				return OpenBrackets(syntactic);
			}
			else if (tok.GetLexeme() == "{")
			{
				SyntaxState* state = new Syn_Statements(m_hasReturn);
				eRETURN_STATE r = state->Update(syntactic);
				if (r == eRETURN_STATE::GOOD)
				{
					return CloseBrackets(syntactic);
				}
				else if (r == eRETURN_STATE::BAD)
				{
					syntactic->Putback(1);
					tok = syntactic->GetNextToken();
					if (tok.GetLexeme() == "}")
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
		return eRETURN_STATE::BAD;
	}
	eRETURN_STATE Syn_ForState::Condition(AnalizadorSintactico* syntactic)
	{
		SyntaxState* state = new Syn_LogicExpresion();
		eRETURN_STATE r = state->Update(syntactic);
		if (r == eRETURN_STATE::GOOD)
		{
			return Semicolon(syntactic);
		}
		else if (r == eRETURN_STATE::BAD)
		{
			Token tok = syntactic->GetNextToken();
			if (tok.GetLexeme() == ")")
			{
				return OpenBrackets(syntactic);
			}
			else if (tok.GetLexeme() == "{")
			{
				SyntaxState* state = new Syn_Statements(m_hasReturn);
				eRETURN_STATE r = state->Update(syntactic);
				if (r == eRETURN_STATE::GOOD)
				{
					return CloseBrackets(syntactic);
				}
				else if (r == eRETURN_STATE::BAD)
				{
					syntactic->Putback(1);
					tok = syntactic->GetNextToken();
					if (tok.GetLexeme() == "}")
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
		return eRETURN_STATE::BAD;
	}
	eRETURN_STATE Syn_ForState::Semicolon(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();
		if (tok.GetLexeme() == ";")
		{
			return IncDecVariable(syntactic);
		}
		else
		{
			std::string errorMsg = "Expected ';' after condition in for on line ";
			errorMsg.append(to_string(tok.GetLine()));
			if (!syntactic->AddError(errorMsg))
			{
				return eRETURN_STATE::FATAL;
			}

			//Panik Mode
			while (tok.GetLexeme() != ")" && tok.GetLexeme() != "{" && tok.GetLexeme() != "}")
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
				if (r == eRETURN_STATE::GOOD)
				{
					return CloseBrackets(syntactic);
				}
				else if (r == eRETURN_STATE::BAD)
				{
					syntactic->Putback(1);
					tok = syntactic->GetNextToken();
					if (tok.GetLexeme() == "}")
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
			else if (tok.GetLexeme() == "}")
			{
				return eRETURN_STATE::GOOD;
			}
		}
		return eRETURN_STATE();
	}
	eRETURN_STATE Syn_ForState::IncDecVariable(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();
		if (tok.GetLexeme() == "inc" || tok.GetLexeme() == "dec")
		{
			return GetIncDecID(syntactic);
		}
		else
		{
			std::string errorMsg = "Expected inc/dec key word after condition in for on line ";
			errorMsg.append(to_string(tok.GetLine()));
			if (!syntactic->AddError(errorMsg))
			{
				return eRETURN_STATE::FATAL;
			}

			//Panik Mode
			while (tok.GetLexeme() != ")" && tok.GetLexeme() != "{" && tok.GetLexeme() != "}")
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
				if (r == eRETURN_STATE::GOOD)
				{
					return CloseBrackets(syntactic);
				}
				else if (r == eRETURN_STATE::BAD)
				{
					syntactic->Putback(1);
					tok = syntactic->GetNextToken();
					if (tok.GetLexeme() == "}")
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
			else if (tok.GetLexeme() == "}")
			{
				return eRETURN_STATE::GOOD;
			}
		}
		return eRETURN_STATE::BAD;
	}
	eRETURN_STATE Syn_ForState::GetIncDecID(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();
		if (tok.GetType() == eTOKEN_TYPE::INT_CONST)
		{
			return CloseParenthesis(syntactic);
		}
		else
		{
			std::string errorMsg = "Expected int to inc/dec in for on line ";
			errorMsg.append(to_string(tok.GetLine()));
			if (!syntactic->AddError(errorMsg))
			{
				return eRETURN_STATE::FATAL;
			}

			//Panik Mode
			while (tok.GetLexeme() != ")" && tok.GetLexeme() != "{" && tok.GetLexeme() != "}")
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
				if (r == eRETURN_STATE::GOOD)
				{
					return CloseBrackets(syntactic);
				}
				else if (r == eRETURN_STATE::BAD)
				{
					syntactic->Putback(1);
					tok = syntactic->GetNextToken();
					if (tok.GetLexeme() == "}")
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
			else if (tok.GetLexeme() == "}")
			{
				return eRETURN_STATE::GOOD;
			}
		}
		return eRETURN_STATE::BAD;
	}
	eRETURN_STATE Syn_ForState::CloseParenthesis(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();
		if (tok.GetLexeme() == ")")
		{
			return OpenBrackets(syntactic);
		}
		else
		{
			std::string errorMsg = "Expected inc/dec key word after condition in for on line ";
			errorMsg.append(to_string(tok.GetLine()));
			if (!syntactic->AddError(errorMsg))
			{
				return eRETURN_STATE::FATAL;
			}

			//Panik Mode
			while (tok.GetLexeme() != ")" && tok.GetLexeme() != "{" && tok.GetLexeme() != "}")
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
				if (r == eRETURN_STATE::GOOD)
				{
					return CloseBrackets(syntactic);
				}
				else if (r == eRETURN_STATE::BAD)
				{
					syntactic->Putback(1);
					tok = syntactic->GetNextToken();
					if (tok.GetLexeme() == "}")
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
			else if (tok.GetLexeme() == "}")
			{
				return eRETURN_STATE::GOOD;
			}
		}
		return eRETURN_STATE::BAD;
	}
	eRETURN_STATE Syn_ForState::OpenBrackets(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();
		if (tok.GetLexeme() == "{")
		{
			SyntaxState* state = new Syn_Statements(m_hasReturn);
			eRETURN_STATE r = state->Update(syntactic);
			if (r == eRETURN_STATE::GOOD)
			{
				return CloseBrackets(syntactic);
			}
			else if (r == eRETURN_STATE::BAD)
			{
				syntactic->Putback(1);
				tok = syntactic->GetNextToken();
				if (tok.GetLexeme() == "}")
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
			std::string errorMsg = "Expected '{' at the beginin of for statements on line ";
			errorMsg.append(to_string(tok.GetLine()));
			if (!syntactic->AddError(errorMsg))
			{
				return eRETURN_STATE::FATAL;
			}

			//Panik Mode
			while (tok.GetLexeme() != "{" && tok.GetLexeme() != "}")
			{
				tok = syntactic->GetNextToken();
			}
			if (tok.GetLexeme() == "{")
			{
				SyntaxState* state = new Syn_Statements(m_hasReturn);
				eRETURN_STATE r = state->Update(syntactic);
				if (r == eRETURN_STATE::GOOD)
				{
					return CloseBrackets(syntactic);
				}
				else if (r == eRETURN_STATE::BAD)
				{
					syntactic->Putback(1);
					tok = syntactic->GetNextToken();
					if (tok.GetLexeme() == "}")
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
			else if (tok.GetLexeme() == "}")
			{
				return eRETURN_STATE::GOOD;
			}
		}
		return eRETURN_STATE::BAD;
	}
	eRETURN_STATE Syn_ForState::CloseBrackets(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();
		if (tok.GetLexeme() == "}")
		{
			return eRETURN_STATE::GOOD;
		}
		else
		{
			std::string errorMsg = "Expected '}' at the end of for on line ";
			errorMsg.append(to_string(tok.GetLine()));
			if (!syntactic->AddError(errorMsg))
			{
				return eRETURN_STATE::FATAL;
			}

			//Panik Mode
			while (tok.GetLexeme() != "}")
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