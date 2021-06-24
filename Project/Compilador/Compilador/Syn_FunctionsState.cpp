#include "pch.h"
#include "Syn_FunctionsState.h"
#include "Syn_FunctionState.h"

namespace Compilador
{
	Syn_FunctionsState::Syn_FunctionsState()
	{
	}
	Syn_FunctionsState::~Syn_FunctionsState()
	{
	}
	eRETURN_STATE Syn_FunctionsState::Update(AnalizadorSintactico* syntactic)
	{
		Token tok = Token(0, "", eTOKEN_TYPE::NONE);
		SyntaxState* state = nullptr;

		tok = syntactic->GetNextToken();

		while (tok.GetType() == eTOKEN_TYPE::KEY_WORD && tok.GetLexeme() == "function")
		{
			state = new Syn_FunctionState();
			if (state->Update(syntactic) == eRETURN_STATE::FATAL)
			{
				delete state;
				return eRETURN_STATE::FATAL;
			}

			delete state;
			state = nullptr;

			tok = syntactic->GetNextToken();
		}

		syntactic->Putback(1);
		return eRETURN_STATE::GOOD;
	}
}