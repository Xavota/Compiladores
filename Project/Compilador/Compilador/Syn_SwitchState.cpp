#include "pch.h"
#include "Syn_SwitchState.h"

#include "Syn_CaseState.h"
#include "Syn_CaseDefaultState.h"

namespace Compilador
{
	Syn_SwitchState::Syn_SwitchState(bool* hasReturn)
	{
		m_hasReturn = hasReturn;
	}
	Syn_SwitchState::~Syn_SwitchState()
	{
	}
	eRETURN_STATE Syn_SwitchState::Update(AnalizadorSintactico* syntactic)
	{
		return OpenParenthesis(syntactic);

		return eRETURN_STATE::GOOD;
	}
	eRETURN_STATE Syn_SwitchState::OpenParenthesis(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();
		if (tok.GetLexeme() == "(")
		{
			return GetID(syntactic);
		}
		else
		{
			std::string errorMsg = "Expected '(' after 'switch' kew word on line ";
			errorMsg.append(to_string(tok.GetLine()));
			if (!syntactic->AddError(errorMsg))
			{
				return eRETURN_STATE::FATAL;
			}

			//Panik Mode
			while (tok.GetLexeme() == "(" && tok.GetType() != eTOKEN_TYPE::ID 
				&& tok.GetLexeme() != ")" && tok.GetLexeme() != "{" && tok.GetLexeme() != "}"
				&& tok.GetType() != eTOKEN_TYPE::END)
			{
				tok = syntactic->GetNextToken();
			}
			if (tok.GetLexeme() == "(")
			{
				return GetID(syntactic);
			}
			else if (tok.GetType() == eTOKEN_TYPE::ID)
			{
				m_relatedID = tok;
				return CloseParenthesis(syntactic);
			}
			else if (tok.GetLexeme() == ")")
			{
				return OpenBrackets(syntactic);
			}
			else if (tok.GetLexeme() == "{")
			{
				StatementNode* stNode = new StatementNode(eSTATEMENT_TYPE::SWITCH);
				stNode->m_relatedToken = m_relatedID;
				syntactic->StatementTreeAddNode(stNode);
				eRETURN_STATE r = Inside(syntactic);
				syntactic->StatementTreeReturnToParent();
				return r;
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
	eRETURN_STATE Syn_SwitchState::GetID(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();
		if (tok.GetType() == eTOKEN_TYPE::ID)
		{
			m_relatedID = tok;
			return CloseParenthesis(syntactic);
		}
		else
		{
			m_relatedID = Token(tok.GetLine(), "a", eTOKEN_TYPE::ID);


			std::string errorMsg = "Expected ID for switch statement on line ";
			errorMsg.append(to_string(tok.GetLine()));
			if (!syntactic->AddError(errorMsg))
			{
				return eRETURN_STATE::FATAL;
			}

			//Panik Mode
			while (tok.GetType() != eTOKEN_TYPE::ID && tok.GetLexeme() != ")" 
				&& tok.GetLexeme() != "{" && tok.GetLexeme() != "}" 
				&& tok.GetType() != eTOKEN_TYPE::END)
			{
				tok = syntactic->GetNextToken();
			}
			if (tok.GetType() == eTOKEN_TYPE::ID)
			{
				m_relatedID = tok;
				return CloseParenthesis(syntactic);
			}
			else if (tok.GetLexeme() == ")")
			{				
				return OpenBrackets(syntactic);
			}
			else if (tok.GetLexeme() == "{")
			{
				StatementNode* stNode = new StatementNode(eSTATEMENT_TYPE::SWITCH);
				stNode->m_relatedToken = m_relatedID;
				syntactic->StatementTreeAddNode(stNode);
				eRETURN_STATE r = Inside(syntactic);
				syntactic->StatementTreeReturnToParent();
				return r;
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
	eRETURN_STATE Syn_SwitchState::CloseParenthesis(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();
		if (tok.GetLexeme() == ")")
		{
			return OpenBrackets(syntactic);
		}
		else
		{
			std::string errorMsg = "Expected ')' after ID on line ";
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
				StatementNode* stNode = new StatementNode(eSTATEMENT_TYPE::SWITCH);
				stNode->m_relatedToken = m_relatedID;
				syntactic->StatementTreeAddNode(stNode);
				eRETURN_STATE r = Inside(syntactic);
				syntactic->StatementTreeReturnToParent();
				return r;
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
	eRETURN_STATE Syn_SwitchState::OpenBrackets(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();
		if (tok.GetLexeme() == "{")
		{
			StatementNode* stNode = new StatementNode(eSTATEMENT_TYPE::SWITCH);
			stNode->m_relatedToken = m_relatedID;
			syntactic->StatementTreeAddNode(stNode);
			eRETURN_STATE r = Inside(syntactic);
			syntactic->StatementTreeReturnToParent();
			return r;
		}
		else
		{
			std::string errorMsg = "Expected '{' after ID on line ";
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
				StatementNode* stNode = new StatementNode(eSTATEMENT_TYPE::SWITCH);
				stNode->m_relatedToken = m_relatedID;
				syntactic->StatementTreeAddNode(stNode);
				eRETURN_STATE r = Inside(syntactic);
				syntactic->StatementTreeReturnToParent();
				return r;
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
	eRETURN_STATE Syn_SwitchState::Inside(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();
		SyntaxState* state = new Syn_CaseState(m_hasReturn, m_relatedID);
		while (tok.GetLexeme() != "}")
		{
			if (tok.GetLexeme() == "case")
			{
				state->Update(syntactic);
			}
			else if (tok.GetLexeme() == "default")
			{
				SyntaxState* stateD = new Syn_CaseDefaultState(m_hasReturn, m_relatedID);
				stateD->Update(syntactic);
				delete stateD;
			}
			else
			{
				std::string errorMsg = "Expected 'case/default' in switch statement on line ";
				errorMsg.append(to_string(tok.GetLine()));
				if (!syntactic->AddError(errorMsg))
				{
					return eRETURN_STATE::FATAL;
				}

				//Panik Mode
				while (tok.GetLexeme() != "case" && tok.GetLexeme() != "default" 
					&& tok.GetLexeme() != "}" && tok.GetType() != eTOKEN_TYPE::END)
				{
					tok = syntactic->GetNextToken();
				}
				if (tok.GetLexeme() == "case")
				{
					state->Update(syntactic);
				}
				else if (tok.GetLexeme() == "default")
				{
					SyntaxState* stateD = new Syn_CaseDefaultState(m_hasReturn, m_relatedID);
					stateD->Update(syntactic);
					delete stateD;
				}
				if (tok.GetLexeme() == "}")
				{
					syntactic->StatementTreeReturnToParent();
					return eRETURN_STATE::GOOD;
				}
				if (tok.GetType() == eTOKEN_TYPE::END)
				{
					return eRETURN_STATE::BAD;
				}
			}
			tok = syntactic->GetNextToken();
		}
		syntactic->StatementTreeReturnToParent();
		return eRETURN_STATE::GOOD;
	}
}