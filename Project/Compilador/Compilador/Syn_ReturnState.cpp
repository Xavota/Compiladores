#include "pch.h"
#include "Syn_ReturnState.h"
#include "Syn_LogicExpresion.h"

namespace Compilador
{
	Syn_ReturnState::Syn_ReturnState()
	{
	}
	Syn_ReturnState::~Syn_ReturnState()
	{
	}
	eRETURN_STATE Syn_ReturnState::Update(AnalizadorSintactico* syntactic)
	{
		StatementNode* stNode = new StatementNode(eSTATEMENT_TYPE::RETURN);
		stNode->m_relatedToken = Token(0, syntactic->GetContext(), eTOKEN_TYPE::ID);
		syntactic->StatementTreeAddNode(stNode);

		LogExpNode* subLogTree = nullptr;
		SyntaxState* state = new Syn_LogicExpresion(&subLogTree);
		eRETURN_STATE r = state->Update(syntactic);
		delete state;
		if (r == eRETURN_STATE::GOOD)
		{
			syntactic->StatementTreeAddLogicTree(subLogTree);
			syntactic->StatementTreeReturnToParent();

			Token tok = syntactic->GetNextToken();
			if (tok.GetLexeme() == ";")
			{
				return eRETURN_STATE::GOOD;
			}
			else
			{
				std::string errorMsg = "Expected ';' at the end of return statement on line ";
				errorMsg.append(to_string(tok.GetLine()));
				if (!syntactic->AddError(errorMsg))
				{
					return eRETURN_STATE::FATAL;
				}

				// Panik mode
				while (tok.GetLexeme() != ";" && tok.GetLexeme() != "}"
					&& tok.GetType() != eTOKEN_TYPE::END)
				{
					tok = syntactic->GetNextToken();
				}
				if (tok.GetLexeme() == "}" || tok.GetType() == eTOKEN_TYPE::END)
				{
					return eRETURN_STATE::BAD;
				}
				else if (tok.GetLexeme() == ";")
				{
					return eRETURN_STATE::GOOD;
				}
				return eRETURN_STATE::BAD;
			}
		}
		else if (r == eRETURN_STATE::BAD)
		{
			syntactic->StatementTreeAddLogicTree(new LogExpNode(Token(0, "0", eTOKEN_TYPE::INT_CONST), 0));
			syntactic->StatementTreeReturnToParent();

			syntactic->Putback(1);
			Token tok = syntactic->GetNextToken();
			if (tok.GetLexeme() == "}" || tok.GetType() == eTOKEN_TYPE::END)
			{
				return eRETURN_STATE::BAD;
			}
			else if (tok.GetLexeme() == ";")
			{
				return eRETURN_STATE::GOOD;
			}
			return eRETURN_STATE::BAD;
		}
		else if (r == eRETURN_STATE::FATAL)
		{
			syntactic->StatementTreeAddLogicTree(new LogExpNode(Token(0, "0", eTOKEN_TYPE::INT_CONST), 0));
			syntactic->StatementTreeReturnToParent();
			return eRETURN_STATE::FATAL;
		}

		return eRETURN_STATE();
	}
}