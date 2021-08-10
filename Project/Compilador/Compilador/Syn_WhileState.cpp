#include "pch.h"
#include "Syn_WhileState.h"

#include "Syn_LogicExpresion.h"
#include "Syn_Statements.h"

namespace Compilador
{
	Syn_WhileState::Syn_WhileState(bool* hasReturn)
	{
		m_hasReturn = hasReturn;
	}
	Syn_WhileState::~Syn_WhileState()
	{
	}
	eRETURN_STATE Syn_WhileState::Update(AnalizadorSintactico* syntactic)
	{
		return OpenParenthesis(syntactic);
	}
	eRETURN_STATE Syn_WhileState::OpenParenthesis(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();
		if (tok.GetLexeme() == "(")
		{
			SyntaxState* state = new Syn_LogicExpresion(&m_subLogExp);
			eRETURN_STATE r = state->Update(syntactic);
			if (r == eRETURN_STATE::GOOD)
			{
				return CloseParenthesis(syntactic);
			}
			else if (r == eRETURN_STATE::BAD)
			{
				syntactic->Putback(1);
				tok = syntactic->GetNextToken();
				if (tok.GetLexeme() == ")")
				{
					return OpenBrackets(syntactic);
				}
				else if (tok.GetLexeme() == "{")
				{
					StatementNode* stNode = new StatementNode(eSTATEMENT_TYPE::WHILE);
					syntactic->StatementTreeAddNode(stNode);
					syntactic->StatementTreeAddLogicTree(new LogExpNode(Token(tok.GetLine(),
						                                   "false", eTOKEN_TYPE::LOGIC_CONST), 0));

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
						syntactic->StatementTreeReturnToParent();
						if (tok.GetLexeme() == "}")
						{
							return eRETURN_STATE::GOOD;
						}
						return eRETURN_STATE::BAD;
					}
					else if (r == eRETURN_STATE::FATAL)
					{
						syntactic->StatementTreeReturnToParent();
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
				return eRETURN_STATE::BAD;
			}
			else if (r == eRETURN_STATE::FATAL)
			{
				return eRETURN_STATE::FATAL;
			}
		}
		else
		{
			std::string errorMsg = "Expected '(' after 'while' key word on line ";
			errorMsg.append(to_string(tok.GetLine()));
			if (!syntactic->AddError(errorMsg))
			{
				return eRETURN_STATE::FATAL;
			}

			//Panik Mode
			while (tok.GetLexeme() != "(" && tok.GetLexeme() != ")" && tok.GetLexeme() != "{"
				&& tok.GetLexeme() != "}" && tok.GetType() != eTOKEN_TYPE::END)
			{
				tok = syntactic->GetNextToken();
			}
			if (tok.GetLexeme() == "(")
			{
				SyntaxState* state = new Syn_LogicExpresion(&m_subLogExp);
				eRETURN_STATE r = state->Update(syntactic);
				if (r == eRETURN_STATE::GOOD)
				{
					return CloseParenthesis(syntactic);
				}
				else if (r == eRETURN_STATE::BAD)
				{
					syntactic->Putback(1);
					tok = syntactic->GetNextToken();
					if (tok.GetLexeme() == ")")
					{
						return OpenBrackets(syntactic);
					}
					else if (tok.GetLexeme() == "{")
					{
						StatementNode* stNode = new StatementNode(eSTATEMENT_TYPE::WHILE);
						syntactic->StatementTreeAddNode(stNode);
						syntactic->StatementTreeAddLogicTree(new LogExpNode(Token(tok.GetLine(),
							                               "false", eTOKEN_TYPE::LOGIC_CONST), 0));

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
							syntactic->StatementTreeReturnToParent();
							if (tok.GetLexeme() == "}")
							{
								return eRETURN_STATE::GOOD;
							}
							return eRETURN_STATE::BAD;
						}
						else if (r == eRETURN_STATE::FATAL)
						{
							syntactic->StatementTreeReturnToParent();
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
				StatementNode* stNode = new StatementNode(eSTATEMENT_TYPE::WHILE);
				syntactic->StatementTreeAddNode(stNode);
				syntactic->StatementTreeAddLogicTree(new LogExpNode(Token(tok.GetLine(),
					                                       "false", eTOKEN_TYPE::LOGIC_CONST), 0));

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
					syntactic->StatementTreeReturnToParent();
					if (tok.GetLexeme() == "}")
					{
						return eRETURN_STATE::GOOD;
					}
					return eRETURN_STATE::BAD;
				}
				else if (r == eRETURN_STATE::FATAL)
				{
					syntactic->StatementTreeReturnToParent();
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
			return eRETURN_STATE::BAD;
		}
		return eRETURN_STATE::BAD;
	}
	eRETURN_STATE Syn_WhileState::CloseParenthesis(AnalizadorSintactico* syntactic)
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
			if (!syntactic->AddError(errorMsg))
			{
				return eRETURN_STATE::FATAL;
			}

			//Panik mode
			while (tok.GetLexeme() != ")" && tok.GetLexeme() != "{" && tok.GetLexeme() != "}"
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
				StatementNode* stNode = new StatementNode(eSTATEMENT_TYPE::WHILE);
				syntactic->StatementTreeAddNode(stNode);
				if (m_subLogExp != nullptr)
				{
					syntactic->StatementTreeAddLogicTree(m_subLogExp);
				}
				else
				{
					syntactic->StatementTreeAddLogicTree(new LogExpNode(Token(tok.GetLine(),
						                                   "false", eTOKEN_TYPE::LOGIC_CONST), 0));
				}

				SyntaxState* state = new Syn_Statements(m_hasReturn);
				eRETURN_STATE r = state->Update(syntactic);
				delete state;
				if (r == eRETURN_STATE::GOOD)
				{
					return CloseBrackets(syntactic);
				}
				else if (r == eRETURN_STATE::BAD)
				{
					syntactic->Putback(1);
					tok = syntactic->GetNextToken();
					syntactic->StatementTreeReturnToParent();
					if (tok.GetLexeme() == "}")
					{
						return eRETURN_STATE::GOOD;
					}
					return eRETURN_STATE::BAD;
				}
				else if (r == eRETURN_STATE::FATAL)
				{
					syntactic->StatementTreeReturnToParent();
					return eRETURN_STATE::FATAL;
				}
				return eRETURN_STATE::BAD;
			}
			else if (tok.GetLexeme() == "}")
			{
				return eRETURN_STATE::GOOD;
			}
			else if (tok.GetType() == eTOKEN_TYPE::END)
			{
				return eRETURN_STATE::BAD;
			}
			return eRETURN_STATE::BAD;
		}
		return eRETURN_STATE::BAD;
	}
	eRETURN_STATE Syn_WhileState::OpenBrackets(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();
		if (tok.GetLexeme() == "{")
		{
			StatementNode* stNode = new StatementNode(eSTATEMENT_TYPE::WHILE);
			syntactic->StatementTreeAddNode(stNode);
			if (m_subLogExp != nullptr)
			{
				syntactic->StatementTreeAddLogicTree(m_subLogExp);
			}
			else
			{
				syntactic->StatementTreeAddLogicTree(new LogExpNode(Token(tok.GetLine(),
					                                       "false", eTOKEN_TYPE::LOGIC_CONST), 0));
			}

			SyntaxState* state = new Syn_Statements(m_hasReturn);
			eRETURN_STATE r = state->Update(syntactic);
			delete state;
			if (r == eRETURN_STATE::GOOD)
			{
				return CloseBrackets(syntactic);
			}
			else if (r == eRETURN_STATE::BAD)
			{
				syntactic->Putback(1);
				tok = syntactic->GetNextToken();
				syntactic->StatementTreeReturnToParent();
				if (tok.GetLexeme() == "}")
				{
					return eRETURN_STATE::GOOD;
				}
				return eRETURN_STATE::BAD;
			}
			else if (r == eRETURN_STATE::FATAL)
			{
				syntactic->StatementTreeReturnToParent();
				return eRETURN_STATE::FATAL;
			}
			return eRETURN_STATE::BAD;
		}
		else
		{
			std::string errorMsg = "Expected '{' before block of statements on line ";
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
				StatementNode* stNode = new StatementNode(eSTATEMENT_TYPE::WHILE);
				syntactic->StatementTreeAddNode(stNode);
				if (m_subLogExp != nullptr)
				{
					syntactic->StatementTreeAddLogicTree(m_subLogExp);
				}
				else
				{
					syntactic->StatementTreeAddLogicTree(new LogExpNode(Token(tok.GetLine(),
						                                   "false", eTOKEN_TYPE::LOGIC_CONST), 0));
				}

				SyntaxState* state = new Syn_Statements(m_hasReturn);
				eRETURN_STATE r = state->Update(syntactic);
				delete state;
				if (r == eRETURN_STATE::GOOD)
				{
					return CloseBrackets(syntactic);
				}
				else if (r == eRETURN_STATE::BAD)
				{
					syntactic->Putback(1);
					tok = syntactic->GetNextToken();
					syntactic->StatementTreeReturnToParent();
					if (tok.GetLexeme() == "}")
					{
						return eRETURN_STATE::GOOD;
					}
					return eRETURN_STATE::BAD;
				}
				else if (r == eRETURN_STATE::FATAL)
				{
					syntactic->StatementTreeReturnToParent();
					return eRETURN_STATE::FATAL;
				}
				return eRETURN_STATE::BAD;
			}
			else if (tok.GetLexeme() == "}")
			{
				return eRETURN_STATE::GOOD;
			}
			else if (tok.GetType() == eTOKEN_TYPE::END)
			{
				return eRETURN_STATE::BAD;
			}
			return eRETURN_STATE::BAD;
		}
		return eRETURN_STATE::BAD;
	}
	eRETURN_STATE Syn_WhileState::CloseBrackets(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();
		if (tok.GetLexeme() == "}")
		{
			syntactic->StatementTreeReturnToParent();
			return eRETURN_STATE::GOOD;
		}
		else
		{
			std::string errorMsg = "Expected '}' after logic expresion on line ";
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
				syntactic->StatementTreeReturnToParent();
				return eRETURN_STATE::GOOD;
			}
			else if (tok.GetType() == eTOKEN_TYPE::END)
			{
				syntactic->StatementTreeReturnToParent();
				return eRETURN_STATE::BAD;
			}
			return eRETURN_STATE::BAD;
		}
		return eRETURN_STATE::BAD;
	}
}
