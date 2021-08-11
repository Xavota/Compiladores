#include "pch.h"
#include "Syn_AsignState.h"

#include "Syn_LogicExpresion.h"

namespace Compilador
{
	Syn_AsignState::Syn_AsignState()
	{
	}
	Syn_AsignState::~Syn_AsignState()
	{
	}
	eRETURN_STATE Syn_AsignState::Update(AnalizadorSintactico* syntactic)
	{
		m_tok = syntactic->GetNextToken();

		Token tok = syntactic->GetNextToken();
		syntactic->Putback(1);
		if (tok.GetLexeme() == "=")
		{
			return Equal(syntactic);
		}
		else
		{
			return Dim(syntactic);
		}
	}

	eRETURN_STATE Syn_AsignState::Dim(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();
		if (tok.GetLexeme() == "[")
		{
			SyntaxState* state = new Syn_LogicExpresion(&m_dimLogExp);
			eRETURN_STATE r = state->Update(syntactic);
			delete state;

			if (r == eRETURN_STATE::GOOD)
			{
				return CloseSquareBrackets(syntactic);
			}
			else if (r == eRETURN_STATE::BAD)
			{
				syntactic->Putback(1);
				tok = syntactic->GetNextToken();
				if (tok.GetLexeme() == ";")
				{
					return eRETURN_STATE::GOOD;
				}
				else if (tok.GetLexeme() == "]")
				{
					m_dimLogExp = new LogExpNode(Token(tok.GetLine(), "0", 
					                                                   eTOKEN_TYPE::INT_CONST), 0);
					return Equal(syntactic);
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
			std::string errorMsg = "Expected '=' or '[' after ID on asignment on line ";
			errorMsg.append(to_string(tok.GetLine()));
			if (!syntactic->AddError(errorMsg))
			{
				return eRETURN_STATE::FATAL;
			}

			//Panik mode
			while (tok.GetLexeme() != ";" && tok.GetLexeme() != "}" && tok.GetLexeme() != "="
				&& tok.GetLexeme() != "[" && tok.GetType() != eTOKEN_TYPE::END)
			{
				tok = syntactic->GetNextToken();
			}
			if (tok.GetLexeme() == "=")
			{
				SyntaxState* state = new Syn_LogicExpresion(&m_subLogExp);
				eRETURN_STATE r = state->Update(syntactic);
				delete state;
				if (r == eRETURN_STATE::GOOD)
				{
					SemiColon(syntactic);
				}
				else if (r == eRETURN_STATE::BAD)
				{
					syntactic->Putback(1);
					tok = syntactic->GetNextToken();
					if (tok.GetLexeme() == ";")
					{
						StatementNode* stNode = new StatementNode(eSTATEMENT_TYPE::ASIGN);
						stNode->m_relatedToken = m_tok;
						syntactic->StatementTreeAddNode(stNode);
						syntactic->StatementTreeAddLogicTree(new LogExpNode(Token(tok.GetLine(),
							                                     "0", eTOKEN_TYPE::INT_CONST), 0));
						syntactic->StatementTreeReturnToParent();
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
			else if (tok.GetLexeme() == "[")
			{
				SyntaxState* state = new Syn_LogicExpresion(&m_dimLogExp);
				eRETURN_STATE r = state->Update(syntactic);
				delete state;

				if (r == eRETURN_STATE::GOOD)
				{
					return CloseSquareBrackets(syntactic);
				}
				else if (r == eRETURN_STATE::BAD)
				{
					syntactic->Putback(1);
					tok = syntactic->GetNextToken();
					if (tok.GetLexeme() == ";")
					{
						return eRETURN_STATE::GOOD;
					}
					else if (tok.GetLexeme() == "]")
					{
						m_dimLogExp = new LogExpNode(Token(tok.GetLine(), "0",
							eTOKEN_TYPE::INT_CONST), 0);
						return Equal(syntactic);
					}
					return eRETURN_STATE::BAD;
				}
				else if (r == eRETURN_STATE::FATAL)
				{
					return eRETURN_STATE::FATAL;
				}
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

	eRETURN_STATE Syn_AsignState::CloseSquareBrackets(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();
		if (tok.GetLexeme() == "]")
		{
			return Equal(syntactic);
		}
		else
		{
			std::string errorMsg = "Expected ']' after dimension on line ";
			errorMsg.append(to_string(tok.GetLine()));
			if (!syntactic->AddError(errorMsg))
			{
				return eRETURN_STATE::FATAL;
			}

			//Panik mode
			while (tok.GetLexeme() != ";" && tok.GetLexeme() != "}" && tok.GetLexeme() != "="
				&& tok.GetLexeme() != "]" && tok.GetType() != eTOKEN_TYPE::END)
			{
				tok = syntactic->GetNextToken();
			}
			if (tok.GetLexeme() == "=")
			{
				SyntaxState* state = new Syn_LogicExpresion(&m_subLogExp);
				eRETURN_STATE r = state->Update(syntactic);
				delete state;
				if (r == eRETURN_STATE::GOOD)
				{
					SemiColon(syntactic);
				}
				else if (r == eRETURN_STATE::BAD)
				{
					syntactic->Putback(1);
					tok = syntactic->GetNextToken();
					if (tok.GetLexeme() == ";")
					{
						StatementNode* stNode = new StatementNode(eSTATEMENT_TYPE::ASIGN);
						stNode->m_relatedToken = m_tok;
						syntactic->StatementTreeAddNode(stNode);
						syntactic->StatementTreeAddLogicTree(m_dimLogExp);
						stNode->m_extraInfo = eEXTRA_INFO::ASING_DIM;
						syntactic->StatementTreeAddLogicTree(new LogExpNode(Token(tok.GetLine(),
							"0", eTOKEN_TYPE::INT_CONST), 0));
						syntactic->StatementTreeReturnToParent();
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
			else if (tok.GetLexeme() == "]")
			{
				return Equal(syntactic);
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

	eRETURN_STATE Syn_AsignState::Equal(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();
		if (tok.GetLexeme() == "=")
		{
			SyntaxState* state = new Syn_LogicExpresion(&m_subLogExp);
			eRETURN_STATE r = state->Update(syntactic);
			delete state;
			if (r == eRETURN_STATE::GOOD)
			{
				SemiColon(syntactic);
			}
			else if (r == eRETURN_STATE::BAD)
			{
				syntactic->Putback(1);
				tok = syntactic->GetNextToken();
				if (tok.GetLexeme() == ";")
				{
					StatementNode* stNode = new StatementNode(eSTATEMENT_TYPE::ASIGN);
					stNode->m_relatedToken = m_tok;
					syntactic->StatementTreeAddNode(stNode);
					if (m_dimLogExp != nullptr)
					{
						syntactic->StatementTreeAddLogicTree(m_dimLogExp);
						stNode->m_extraInfo = eEXTRA_INFO::ASING_DIM;
					}
					syntactic->StatementTreeAddLogicTree(new LogExpNode(Token(tok.GetLine(),
						                                         "0", eTOKEN_TYPE::INT_CONST), 0));
					syntactic->StatementTreeReturnToParent();
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
			std::string errorMsg = "Expected '=' after ']' on asignment on line ";
			errorMsg.append(to_string(tok.GetLine()));
			if (!syntactic->AddError(errorMsg))
			{
				return eRETURN_STATE::FATAL;
			}

			//Panik mode
			while (tok.GetLexeme() != ";" && tok.GetLexeme() != "}" && tok.GetLexeme() != "=" 
			    && tok.GetType() != eTOKEN_TYPE::END)
			{
				tok = syntactic->GetNextToken();
			}
			if (tok.GetLexeme() == "=")
			{
				SyntaxState* state = new Syn_LogicExpresion(&m_subLogExp);
				eRETURN_STATE r = state->Update(syntactic);
				delete state;
				if (r == eRETURN_STATE::GOOD)
				{
					SemiColon(syntactic);
				}
				else if (r == eRETURN_STATE::BAD)
				{
					syntactic->Putback(1);
					tok = syntactic->GetNextToken();
					if (tok.GetLexeme() == ";")
					{
						StatementNode* stNode = new StatementNode(eSTATEMENT_TYPE::ASIGN);
						stNode->m_relatedToken = m_tok;
						syntactic->StatementTreeAddNode(stNode);
						if (m_dimLogExp != nullptr)
						{
							syntactic->StatementTreeAddLogicTree(m_dimLogExp);
							stNode->m_extraInfo = eEXTRA_INFO::ASING_DIM;
						}
						syntactic->StatementTreeAddLogicTree(new LogExpNode(Token(tok.GetLine(),
							                                     "0", eTOKEN_TYPE::INT_CONST), 0));
						syntactic->StatementTreeReturnToParent();
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

	eRETURN_STATE Syn_AsignState::SemiColon(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();
		if (tok.GetLexeme() == ";")
		{
			StatementNode* stNode = new StatementNode(eSTATEMENT_TYPE::ASIGN);
			stNode->m_relatedToken = m_tok;
			syntactic->StatementTreeAddNode(stNode);
			if (m_dimLogExp != nullptr)
			{
				syntactic->StatementTreeAddLogicTree(m_dimLogExp);
				stNode->m_extraInfo = eEXTRA_INFO::ASING_DIM;
			}
			syntactic->StatementTreeAddLogicTree(m_subLogExp);
			syntactic->StatementTreeReturnToParent();
			return eRETURN_STATE::GOOD;
		}
		else
		{
			std::string errorMsg = "Expected ';' after logic expresion on asignment on line ";
			errorMsg.append(to_string(tok.GetLine()));
			if (!syntactic->AddError(errorMsg))
			{
				return eRETURN_STATE::FATAL;
			}

			//Panik mode
			while (tok.GetLexeme() != ";" && tok.GetLexeme() != "}"
				&& tok.GetType() != eTOKEN_TYPE::END)
			{
				tok = syntactic->GetNextToken();
			}
			if (tok.GetLexeme() == ";")
			{
				StatementNode* stNode = new StatementNode(eSTATEMENT_TYPE::ASIGN);
				stNode->m_relatedToken = m_tok;
				syntactic->StatementTreeAddNode(stNode);
				if (m_dimLogExp != nullptr)
				{
					syntactic->StatementTreeAddLogicTree(m_dimLogExp);
					stNode->m_extraInfo = eEXTRA_INFO::ASING_DIM;
				}
				syntactic->StatementTreeAddLogicTree(m_subLogExp);
				syntactic->StatementTreeReturnToParent();
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
