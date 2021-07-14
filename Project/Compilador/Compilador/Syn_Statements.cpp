#include "pch.h"
#include "Syn_Statements.h"

#include "Syn_IfState.h"
#include "Syn_ReturnState.h"

namespace Compilador
{
	Syn_Statements::Syn_Statements(bool* hasReturn)
	{
		m_hasReturn = hasReturn;
	}
	Syn_Statements::~Syn_Statements()
	{
	}
	eRETURN_STATE Syn_Statements::Update(AnalizadorSintactico* syntactic)
	{
		bool finished = false;

		// Variables

		Token tok = Token(0, "", eTOKEN_TYPE::NONE);
		SyntaxState* state = nullptr;

		while (!finished)
		{
			tok = syntactic->GetNextToken();

			state = nullptr;

			if (tok.GetLexeme() == "if")
			{
				state = new Syn_IfState(m_hasReturn);
				if (state->Update(syntactic) == eRETURN_STATE::FATAL)
				{
					delete state;
					return eRETURN_STATE::FATAL;
				}

				delete state;
				state = nullptr;

				continue;
			}
			else
			{
				syntactic->Putback(1);
			}


			// Functions

			tok = syntactic->GetNextToken();

			if (tok.GetLexeme() == "return")
			{
				state = new Syn_ReturnState();
				eRETURN_STATE r = state->Update(syntactic);
				if (r == eRETURN_STATE::FATAL)
				{
					delete state;
					return eRETURN_STATE::FATAL;
				}
				else if (r == eRETURN_STATE::GOOD)
				{
					*m_hasReturn = true;
				}

				delete state;
				state = nullptr;

				continue;
			}
			else
			{
				syntactic->Putback(1);
			}

			// End

			tok = syntactic->GetNextToken();

			if (tok.GetLexeme() == "}")
			{
				finished = true;

				syntactic->Putback(1);

				continue;
			}
			else
			{
				std::string errorMsg = "Invalid Token: ";
				errorMsg.append(tok.GetLexeme());
				errorMsg.append(" on line ");
				errorMsg.append(to_string(tok.GetLine()));
				if (!syntactic->AddError(errorMsg))
				{
					return eRETURN_STATE::FATAL;
				}
			}
		}

		return eRETURN_STATE::GOOD;
	}
}