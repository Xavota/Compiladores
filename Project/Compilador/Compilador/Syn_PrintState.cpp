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
			StatementNode* stNode = new StatementNode(eSTATEMENT_TYPE::PRINT);
			syntactic->StatementTreeAddNode(stNode);
			eRETURN_STATE r = Inside(syntactic);
			syntactic->StatementTreeReturnToParent();
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

			while (tok.GetLexeme() != "(" && tok.GetLexeme() != ")" && tok.GetLexeme() != "}" 
				&& tok.GetLexeme() != ";" && tok.GetType() != eTOKEN_TYPE::END)
			{
				tok = syntactic->GetNextToken();
			}
			if (tok.GetLexeme() == "(")
			{
				StatementNode* stNode = new StatementNode(eSTATEMENT_TYPE::PRINT);
				syntactic->StatementTreeAddNode(stNode);
				eRETURN_STATE r = Inside(syntactic);
				syntactic->StatementTreeReturnToParent();
				if (r == eRETURN_STATE::GOOD)
				{
					return Semicolon(syntactic);
				}
				else
				{
					return r;
				}
			}
			else if (tok.GetLexeme() == ")")
			{
				return Semicolon(syntactic);
			}
			else if (tok.GetLexeme() == ";")
			{
				return eRETURN_STATE::GOOD;
			}
			else if (tok.GetLexeme() == "}" || tok.GetType() == eTOKEN_TYPE::END)
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
		SyntaxState* state = nullptr;
		while (true)
		{
			m_prints.push_back(nullptr);
			state = new Syn_LogicExpresion(&m_prints[m_prints.size() - 1]);
			eRETURN_STATE r = state->Update(syntactic);
			if (r == eRETURN_STATE::FATAL)
			{
				return eRETURN_STATE::FATAL;
			}
			else if (r == eRETURN_STATE::BAD)
			{
				syntactic->Putback(1);
				tok = syntactic->GetNextToken();
				if (tok.GetLexeme() == "}" || tok.GetType() == eTOKEN_TYPE::END)
				{
					return eRETURN_STATE::BAD;
				}
				else if (tok.GetLexeme() == ")")
				{
					syntactic->StatementTreeAddLogicTree(new LogExpNode(Token(tok.GetLine(), 
					                                      "\"a\"", eTOKEN_TYPE::STRING_CONST), 0));
					return eRETURN_STATE::GOOD;
				}
				else if (tok.GetLexeme() == ";")
				{
					syntactic->Putback(1);
					return eRETURN_STATE::GOOD;
				}
				return eRETURN_STATE::BAD;
			}
			else if (r == eRETURN_STATE::GOOD)
			{
				syntactic->StatementTreeAddLogicTree(m_prints[m_prints.size() - 1]);
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

				// Panik mode
				while (tok.GetLexeme() != ")" && tok.GetLexeme() != "}" && tok.GetLexeme() != ";"
					&& tok.GetType() != eTOKEN_TYPE::END)
				{
					tok = syntactic->GetNextToken();
				}
				if (tok.GetLexeme() == ")")
				{
					return eRETURN_STATE::GOOD;
				}
				else if (tok.GetLexeme() == ";")
				{
					syntactic->Putback(1);
					return eRETURN_STATE::GOOD;
				}
				else if (tok.GetLexeme() == "}" || tok.GetType() == eTOKEN_TYPE::END)
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

			while (tok.GetLexeme() != "}" && tok.GetLexeme() != ";"
				&& tok.GetType() != eTOKEN_TYPE::END)
			{
				tok = syntactic->GetNextToken();
			}
			if (tok.GetLexeme() == ";")
			{
				return eRETURN_STATE::GOOD;
			}
			else if (tok.GetLexeme() == "}" || tok.GetType() == eTOKEN_TYPE::END)
			{ 
				return eRETURN_STATE::BAD;
			}
			return eRETURN_STATE::BAD;
		}
		return eRETURN_STATE::BAD;
	}
}