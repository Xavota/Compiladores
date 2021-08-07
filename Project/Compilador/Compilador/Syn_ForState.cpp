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
			std::string errorMsg = "Expected '(' after 'for' key word on line ";
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
				return VariableAsignation(syntactic);
			}
			else if (tok.GetLexeme() == ")")
			{
				return OpenBrackets(syntactic);
			}
			else if (tok.GetLexeme() == "{")
			{
				StatementNode* stNode = new StatementNode(eSTATEMENT_TYPE::FOR);
				stNode->m_extraInfo == eEXTRA_INFO::FOR_INC;
				stNode->m_forIncDec = 1;
				stNode->m_relatedToken = Token(tok.GetLine(), "i", eTOKEN_TYPE::ID);
				syntactic->StatementTreeAddNode(stNode);
				syntactic->StatementTreeAddLogicTree(new LogExpNode(Token(tok.GetLine(), "false", 
													                eTOKEN_TYPE::LOGIC_CONST), 0));

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
	eRETURN_STATE Syn_ForState::VariableAsignation(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();
		if (tok.GetType() == eTOKEN_TYPE::ID)
		{
			m_incDecTok = tok;
		}
		else
		{
			m_incDecTok = Token(tok.GetLine(), "i", eTOKEN_TYPE::ID);
		}
		syntactic->Putback(1);

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
				StatementNode* stNode = new StatementNode(eSTATEMENT_TYPE::FOR);
				stNode->m_extraInfo == eEXTRA_INFO::FOR_INC;
				stNode->m_forIncDec = 1;
				stNode->m_relatedToken = m_incDecTok;
				syntactic->StatementTreeAddNode(stNode);
				syntactic->StatementTreeAddLogicTree(new LogExpNode(Token(tok.GetLine(), "false",
					eTOKEN_TYPE::LOGIC_CONST), 0));

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
		else if (r == eRETURN_STATE::FATAL)
		{
			return eRETURN_STATE::FATAL;
		}
		return eRETURN_STATE::BAD;
	}
	eRETURN_STATE Syn_ForState::Condition(AnalizadorSintactico* syntactic)
	{
		SyntaxState* state = new Syn_LogicExpresion(&m_subeLogExp);
		eRETURN_STATE r = state->Update(syntactic);
		if (r == eRETURN_STATE::GOOD)
		{
			return Semicolon(syntactic);
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
				StatementNode* stNode = new StatementNode(eSTATEMENT_TYPE::FOR);
				stNode->m_extraInfo == eEXTRA_INFO::FOR_INC;
				stNode->m_forIncDec = 1;
				stNode->m_relatedToken = m_incDecTok;
				syntactic->StatementTreeAddNode(stNode);
				syntactic->StatementTreeAddLogicTree(new LogExpNode(Token(tok.GetLine(), "false",
					eTOKEN_TYPE::LOGIC_CONST), 0));

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
			while (tok.GetLexeme() != ")" && tok.GetLexeme() != "{" && tok.GetLexeme() != "}"
				&& tok.GetLexeme() != ";" && tok.GetType() != eTOKEN_TYPE::END)
			{
				tok = syntactic->GetNextToken();
			}
			if (tok.GetLexeme() == ";")
			{
				return IncDecVariable(syntactic);
			}
			if (tok.GetLexeme() == ")")
			{
				return OpenBrackets(syntactic);
			}
			else if (tok.GetLexeme() == "{")
			{
				StatementNode* stNode = new StatementNode(eSTATEMENT_TYPE::FOR);
				stNode->m_extraInfo == eEXTRA_INFO::FOR_INC;
				stNode->m_forIncDec = 1;
				stNode->m_relatedToken = m_incDecTok;
				syntactic->StatementTreeAddNode(stNode);
				if (m_subeLogExp != nullptr)
				{
					syntactic->StatementTreeAddLogicTree(m_subeLogExp);
				}
				else
				{
					syntactic->StatementTreeAddLogicTree(new LogExpNode(Token(tok.GetLine(), 
					                                       "false", eTOKEN_TYPE::LOGIC_CONST), 0));
				}

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
	eRETURN_STATE Syn_ForState::IncDecVariable(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();
		if (tok.GetLexeme() == "inc" || tok.GetLexeme() == "dec")
		{
			if (tok.GetLexeme() == "inc")
			{
				m_extraInfo == eEXTRA_INFO::FOR_INC;
			}
			else
			{
				m_extraInfo == eEXTRA_INFO::FOR_DEC;
			}
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
				StatementNode* stNode = new StatementNode(eSTATEMENT_TYPE::FOR);
				stNode->m_extraInfo == eEXTRA_INFO::FOR_INC;
				stNode->m_forIncDec = 1;
				stNode->m_relatedToken = m_incDecTok;
				syntactic->StatementTreeAddNode(stNode);
				if (m_subeLogExp != nullptr)
				{
					syntactic->StatementTreeAddLogicTree(m_subeLogExp);
				}
				else
				{
					syntactic->StatementTreeAddLogicTree(new LogExpNode(Token(tok.GetLine(), 
					                                       "false", eTOKEN_TYPE::LOGIC_CONST), 0));
				}

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

	int StrToInt(Token t, AnalizadorSintactico* syntactic, eRETURN_STATE& r)
	{
		int res = 0;
		int lasRest = 0;
		for (char c : t.GetLexeme())
		{
			res *= 10;
			res += c - '0';

			if (res > lasRest)
			{
				std::string errorMsg = "Integer too big ";
				errorMsg.append(t.GetLexeme());
				errorMsg.append(", on line ");
				errorMsg.append(to_string(t.GetLine()));
				if (!syntactic->AddError(errorMsg))
				{
					r = eRETURN_STATE::FATAL;
				}
				return lasRest;
			}
			lasRest = res;
		}
		return res;
	}

	eRETURN_STATE Syn_ForState::GetIncDecID(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();
		if (tok.GetType() == eTOKEN_TYPE::INT_CONST)
		{
			eRETURN_STATE r = eRETURN_STATE::GOOD;
			m_incDecVal = StrToInt(tok, syntactic, r);
			if (r == eRETURN_STATE::FATAL)
			{
				return eRETURN_STATE::FATAL;
			}
			return CloseParenthesis(syntactic);
		}
		else
		{
			std::string errorMsg = "Expected int after inc/dec in for on line ";
			errorMsg.append(to_string(tok.GetLine()));
			if (!syntactic->AddError(errorMsg))
			{
				return eRETURN_STATE::FATAL;
			}

			//Panik Mode
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
				StatementNode* stNode = new StatementNode(eSTATEMENT_TYPE::FOR);
				stNode->m_extraInfo == m_extraInfo;
				stNode->m_forIncDec = 1;
				stNode->m_relatedToken = m_incDecTok;
				syntactic->StatementTreeAddNode(stNode);
				if (m_subeLogExp != nullptr)
				{
					syntactic->StatementTreeAddLogicTree(m_subeLogExp);
				}
				else
				{
					syntactic->StatementTreeAddLogicTree(new LogExpNode(Token(tok.GetLine(), 
					                                       "false", eTOKEN_TYPE::LOGIC_CONST), 0));
				}

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
				StatementNode* stNode = new StatementNode(eSTATEMENT_TYPE::FOR);
				stNode->m_extraInfo == m_extraInfo;
				stNode->m_forIncDec = m_incDecVal;
				stNode->m_relatedToken = m_incDecTok;
				syntactic->StatementTreeAddNode(stNode);
				if (m_subeLogExp != nullptr)
				{
					syntactic->StatementTreeAddLogicTree(m_subeLogExp);
				}
				else
				{
					syntactic->StatementTreeAddLogicTree(new LogExpNode(Token(tok.GetLine(), 
					                                       "false", eTOKEN_TYPE::LOGIC_CONST), 0));
				}

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
	eRETURN_STATE Syn_ForState::OpenBrackets(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();
		if (tok.GetLexeme() == "{")
		{
			StatementNode* stNode = new StatementNode(eSTATEMENT_TYPE::FOR);
			stNode->m_extraInfo == m_extraInfo;
			stNode->m_forIncDec = m_incDecVal;
			if (m_incDecTok.GetType() != eTOKEN_TYPE::NONE)
			{
				stNode->m_relatedToken = m_incDecTok;
			}
			else
			{
				stNode->m_relatedToken = Token(tok.GetLine(), "i", eTOKEN_TYPE::ID);
			}
			syntactic->StatementTreeAddNode(stNode);
			if (m_subeLogExp != nullptr)
			{
				syntactic->StatementTreeAddLogicTree(m_subeLogExp);
			}
			else
			{
				syntactic->StatementTreeAddLogicTree(new LogExpNode(Token(tok.GetLine(), "false",
					                                                eTOKEN_TYPE::LOGIC_CONST), 0));
			}

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
				return eRETURN_STATE::FATAL;
			}
			return eRETURN_STATE::BAD;
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
			while (tok.GetLexeme() != "{" && tok.GetLexeme() != "}" 
			    && tok.GetType() != eTOKEN_TYPE::END)
			{
				tok = syntactic->GetNextToken();
			}
			if (tok.GetLexeme() == "{")
			{
				StatementNode* stNode = new StatementNode(eSTATEMENT_TYPE::FOR);
				stNode->m_extraInfo == m_extraInfo;
				stNode->m_forIncDec = m_incDecVal;
				if (m_incDecTok.GetType() != eTOKEN_TYPE::NONE)
				{
					stNode->m_relatedToken = m_incDecTok;
				}
				else
				{
					stNode->m_relatedToken = Token(tok.GetLine(), "i", eTOKEN_TYPE::ID);
				}
				syntactic->StatementTreeAddNode(stNode);
				if (m_subeLogExp != nullptr)
				{
					syntactic->StatementTreeAddLogicTree(m_subeLogExp);
				}
				else
				{
					syntactic->StatementTreeAddLogicTree(new LogExpNode(Token(tok.GetLine(), 
					                                       "false", eTOKEN_TYPE::LOGIC_CONST), 0));
				}

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
	eRETURN_STATE Syn_ForState::CloseBrackets(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();
		if (tok.GetLexeme() == "}")
		{
			syntactic->StatementTreeReturnToParent();
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