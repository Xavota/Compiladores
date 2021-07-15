#include "pch.h"
#include "Syn_ProgramState.h"

#include "Syn_VariablesState.h"
#include "Syn_FunctionsState.h"
#include "Syn_MainState.h"

namespace Compilador
{
	Syn_ProgramState::Syn_ProgramState()
	{
	}

	Syn_ProgramState::~Syn_ProgramState()
	{
	}

	eRETURN_STATE Syn_ProgramState::Update(AnalizadorSintactico* syntactic)
	{
		bool hasMain = false;
		bool hasFunctions = false;
		
		bool finished = false;

		// Variables

		Token tok = Token(0, "", eTOKEN_TYPE::NONE);
		SyntaxState* state = nullptr;

		while (!finished)
		{
			tok = syntactic->GetNextToken();

			state = nullptr;

			if (tok.GetType() == eTOKEN_TYPE::KEY_WORD && tok.GetLexeme() == "var")
			{
				if (hasFunctions)
				{
					if (!syntactic->AddError("Global variables must be before functions"))
					{
						return eRETURN_STATE::FATAL;
					}
				}
				if (hasMain)
				{
					if (!syntactic->AddError("Global variables must be before main"))
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

				continue;
			}
			else
			{
				syntactic->Putback(1);
			}


			// Functions

			tok = syntactic->GetNextToken();

			if (tok.GetType() == eTOKEN_TYPE::KEY_WORD && tok.GetLexeme() == "function")
			{
				if (hasMain)
				{
					if (!syntactic->AddError("Functions must be before main"))
					{
						return eRETURN_STATE::FATAL;
					}
				}

				syntactic->Putback(1);
				state = new Syn_FunctionsState();
				if (state->Update(syntactic) == eRETURN_STATE::FATAL)
				{
					delete state;
					return eRETURN_STATE::FATAL;
				}
				hasFunctions = true;

				delete state;
				state = nullptr;

				continue;
			}
			else
			{
				syntactic->Putback(1);
			}


			// main

			tok = syntactic->GetNextToken();

			if (tok.GetType() == eTOKEN_TYPE::KEY_WORD && tok.GetLexeme() == "main")
			{
				syntactic->AddSymbol(tok.GetLine(), "main", "FUNCTION", 0, "", "void");
				syntactic->SetContext("main");

				state = new Syn_MainState();
				if (state->Update(syntactic) == eRETURN_STATE::FATAL)
				{
					delete state;
					return eRETURN_STATE::FATAL;
				}
				hasMain = true;

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

			if (tok.GetType() == eTOKEN_TYPE::END)
			{
				finished = true;

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



		if (!hasMain)
		{
			if (!syntactic->AddError("Main function is missing"))
			{
				return eRETURN_STATE::FATAL;
			}
		}

		return eRETURN_STATE::GOOD;
	}
}