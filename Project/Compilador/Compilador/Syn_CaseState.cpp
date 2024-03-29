#include "pch.h"
#include "Syn_CaseState.h"

#include "Syn_Statements.h"

namespace Compilador
{
	Syn_CaseState::Syn_CaseState(bool* hasReturn, const Token& ID)
	{
		m_hasReturn = hasReturn;
		m_ID = ID;
	}
	Syn_CaseState::~Syn_CaseState()
	{
	}
	eRETURN_STATE Syn_CaseState::Update(AnalizadorSintactico* syntactic)
	{
		return Constant(syntactic);
	}
	eRETURN_STATE Syn_CaseState::Constant(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();
		if (IsCaseType(tok))
		{
			m_log = new LogExpNode(tok, 0);
			return Dobledot(syntactic);
		}
		else
		{
			std::string errorMsg = "Expected a valid ID after case key word on line ";
			errorMsg.append(to_string(tok.GetLine()));
			if (!syntactic->AddError(errorMsg))
			{
				return eRETURN_STATE::FATAL;
			}

			//Panik mode
			while (tok.GetLexeme() != ":" && tok.GetLexeme() != "{" && tok.GetLexeme() != "}"
				&& tok.GetType() != eTOKEN_TYPE::END)
			{
				tok = syntactic->GetNextToken();
			}
			if (tok.GetLexeme() == ":")
			{
				return OpenBrackets(syntactic);
			}
			else if (tok.GetLexeme() == "{")
			{
				StatementNode* stNode = new StatementNode(eSTATEMENT_TYPE::CASE);
				stNode->m_relatedToken = m_ID;
				syntactic->StatementTreeAddNode(stNode);
				if (m_log != nullptr)
				{
					syntactic->StatementTreeAddLogicTree(m_log);
				}
				else
				{
					syntactic->StatementTreeAddLogicTree(new LogExpNode(Token(tok.GetLine(), "0", eTOKEN_TYPE::INT_CONST), 0));
				}

				SyntaxState* state = new Syn_Statements(m_hasReturn);
				eRETURN_STATE r = state->Update(syntactic);
				delete state;

				syntactic->StatementTreeReturnToParent();

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
			else if (tok.GetType() == eTOKEN_TYPE::END)
			{
				return eRETURN_STATE::BAD;
			}
		}
		return eRETURN_STATE::BAD;
	}
	eRETURN_STATE Syn_CaseState::Dobledot(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();
		if (tok.GetLexeme() == ":")
		{
			return OpenBrackets(syntactic);
		}
		else
		{
			std::string errorMsg = "Expected ':' after ID on case on line ";
			errorMsg.append(to_string(tok.GetLine()));
			if (!syntactic->AddError(errorMsg))
			{
				return eRETURN_STATE::FATAL;
			}

			//Panik mode
			while (tok.GetLexeme() != ":" && tok.GetLexeme() != "{" && tok.GetLexeme() != "}"
				&& tok.GetType() != eTOKEN_TYPE::END)
			{
				tok = syntactic->GetNextToken();
			}
			if (tok.GetLexeme() == ":")
			{
				return OpenBrackets(syntactic);
			}
			else if (tok.GetLexeme() == "{")
			{
				StatementNode* stNode = new StatementNode(eSTATEMENT_TYPE::CASE);
				stNode->m_relatedToken = m_ID;
				syntactic->StatementTreeAddNode(stNode);
				if (m_log != nullptr)
				{
					syntactic->StatementTreeAddLogicTree(m_log);
				}
				else
				{
					syntactic->StatementTreeAddLogicTree(new LogExpNode(Token(tok.GetLine(), "0", eTOKEN_TYPE::INT_CONST), 0));
				}

				SyntaxState* state = new Syn_Statements(m_hasReturn);
				eRETURN_STATE r = state->Update(syntactic);
				delete state;

				syntactic->StatementTreeReturnToParent();

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
			else if (tok.GetType() == eTOKEN_TYPE::END)
			{
				return eRETURN_STATE::BAD;
			}
		}
		return eRETURN_STATE::BAD;
	}
	eRETURN_STATE Syn_CaseState::OpenBrackets(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();
		if (tok.GetLexeme() == "{")
		{
			StatementNode* stNode = new StatementNode(eSTATEMENT_TYPE::CASE);
			stNode->m_relatedToken = m_ID;
			syntactic->StatementTreeAddNode(stNode);
			if (m_log != nullptr)
			{
				syntactic->StatementTreeAddLogicTree(m_log);
			}
			else
			{
				syntactic->StatementTreeAddLogicTree(new LogExpNode(Token(tok.GetLine(), "0", eTOKEN_TYPE::INT_CONST), 0));
			}

			SyntaxState* state = new Syn_Statements(m_hasReturn);
			eRETURN_STATE r = state->Update(syntactic);
			delete state;

			syntactic->StatementTreeReturnToParent();

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
			std::string errorMsg = "Expected '{' after ':' on case on line ";
			errorMsg.append(to_string(tok.GetLine()));
			if (!syntactic->AddError(errorMsg))
			{
				return eRETURN_STATE::FATAL;
			}

			//Panik mode
			while (tok.GetLexeme() != "{" && tok.GetLexeme() != "}"
				&& tok.GetType() != eTOKEN_TYPE::END)
			{
				tok = syntactic->GetNextToken();
			}
			if (tok.GetLexeme() == "{")
			{
				StatementNode* stNode = new StatementNode(eSTATEMENT_TYPE::CASE);
				stNode->m_relatedToken = m_ID;
				syntactic->StatementTreeAddNode(stNode);
				if (m_log != nullptr)
				{
					syntactic->StatementTreeAddLogicTree(m_log);
				}
				else
				{
					syntactic->StatementTreeAddLogicTree(new LogExpNode(Token(tok.GetLine(), "0", eTOKEN_TYPE::INT_CONST), 0));
				}

				SyntaxState* state = new Syn_Statements(m_hasReturn);
				eRETURN_STATE r = state->Update(syntactic);
				delete state;

				syntactic->StatementTreeReturnToParent();

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
			else if (tok.GetType() == eTOKEN_TYPE::END)
			{
				return eRETURN_STATE::BAD;
			}
		}
		return eRETURN_STATE::BAD;
	}
	eRETURN_STATE Syn_CaseState::CloseBrackets(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();
		if (tok.GetLexeme() == "}")
		{
			return eRETURN_STATE::GOOD;
		}
		else
		{
			std::string errorMsg = "Expected '}' after statements on case on line ";
			errorMsg.append(to_string(tok.GetLine()));
			if (!syntactic->AddError(errorMsg))
			{
				return eRETURN_STATE::FATAL;
			}

			//Panik mode
			while (tok.GetLexeme() != "}" && tok.GetType() != eTOKEN_TYPE::END)
			{
				tok = syntactic->GetNextToken();
			}
			if (tok.GetLexeme() == "}")
			{
				return eRETURN_STATE::GOOD;
			}
			else if (tok.GetType() == eTOKEN_TYPE::END)
			{
				return eRETURN_STATE::BAD;
			}
		}
		return eRETURN_STATE::BAD;
	}
}