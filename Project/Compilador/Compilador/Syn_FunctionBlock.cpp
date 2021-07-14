#include "pch.h"
#include "Syn_FunctionBlock.h"

#include "Syn_VariablesState.h"
#include "Syn_Statements.h"

namespace Compilador
{
	Syn_FunctionBlock::Syn_FunctionBlock(bool* hasReturn)
	{
		m_hasReturn = hasReturn;
	}
	Syn_FunctionBlock::~Syn_FunctionBlock()
	{
	}
	eRETURN_STATE Syn_FunctionBlock::Update(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();

		bool finished = false;
		bool hasStates = false;
		SyntaxState* state = nullptr;
		while (!finished)
		{
			state = nullptr;

			if (tok.GetType() == eTOKEN_TYPE::KEY_WORD && tok.GetLexeme() == "var")
			{
				if (hasStates)
				{
					if (!syntactic->AddError("Local variables must be after statements"))
					{
						return eRETURN_STATE::FATAL;
					}
				}
				syntactic->Putback(1);
				state = new Syn_VariablesState();
				if (state->Update(syntactic) == eRETURN_STATE::FATAL)
				{
					delete state;
					return eRETURN_STATE::FATAL;
				}

				delete state;
				state = nullptr;

				tok = syntactic->GetNextToken();

				continue;
			}
			else
			{
				syntactic->Putback(1);
			}


			// Satetments

			state = new Syn_Statements(m_hasReturn);
			if (state->Update(syntactic) == eRETURN_STATE::FATAL)
			{
				delete state;
				return eRETURN_STATE::FATAL;
			}
			hasStates = true;

			delete state;
			state = nullptr;


			// End

			tok = syntactic->GetNextToken();

			if (tok.GetLexeme() == "}")
			{
				finished = true;
			}
		}


		return eRETURN_STATE::GOOD;
	}
}