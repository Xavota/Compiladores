#include "pch.h"
#include "Syn_FunctionCallState.h"

#include "Syn_LogicExpresion.h"

namespace Compilador
{
	Syn_FunctionCallState::Syn_FunctionCallState(std::vector<LogExpNode*>* params)
	{
		m_params = params;
	}
	Syn_FunctionCallState::~Syn_FunctionCallState()
	{
	}
	eRETURN_STATE Syn_FunctionCallState::Update(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();

		return OpenParenthesis(syntactic);
	}
	eRETURN_STATE Syn_FunctionCallState::OpenParenthesis(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();
		if (tok.GetLexeme() == "(")
		{
			SyntaxState* state = nullptr;
			while (true)
			{
				m_params->push_back(nullptr);
				state = new Syn_LogicExpresion(&(*m_params)[m_params->size() - 1]);
				eRETURN_STATE r = state->Update(syntactic);
				delete state;
				if (r == eRETURN_STATE::FATAL)
				{
					return eRETURN_STATE::FATAL;
				}
				else if (r == eRETURN_STATE::BAD)
				{
					tok = syntactic->GetNextToken();
					if (tok.GetLexeme() == ";" || tok.GetLexeme() == "}" 
					 || tok.GetType() == eTOKEN_TYPE::END)
					{
						return eRETURN_STATE::BAD;
					}
					else if (tok.GetLexeme() == ")")
					{
						return eRETURN_STATE::GOOD;
					}
					return eRETURN_STATE::BAD;
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
					std::string errorMsg = "Expected ')' at the end of function call on line ";
					errorMsg.append(to_string(tok.GetLine()));
					if (!syntactic->AddError(errorMsg))
					{
						return eRETURN_STATE::FATAL;
					}

					while (tok.GetLexeme() != ")" && tok.GetLexeme() != "}" 
						&& tok.GetLexeme() != ";" && tok.GetType() != eTOKEN_TYPE::END)
					{
						tok = syntactic->GetNextToken();
					}
					if (tok.GetLexeme() == ")")
					{
						return eRETURN_STATE::GOOD;
					}
					else if (tok.GetLexeme() == "}" || tok.GetLexeme() == ";" 
					      || tok.GetType() == eTOKEN_TYPE::END)
					{
						return eRETURN_STATE::BAD;
					}
					return eRETURN_STATE::BAD;
				}
			}
		}

		return eRETURN_STATE::BAD;
	}
}