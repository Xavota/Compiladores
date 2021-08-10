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

		state = new Syn_FunctionState();
		while (tok.GetLexeme() == "function")
		{
			eRETURN_STATE r = state->Update(syntactic);
			if (r == eRETURN_STATE::FATAL)
			{
				delete state;
				return eRETURN_STATE::FATAL;
			}

			tok = syntactic->GetNextToken();
		}
		delete state;
		state = nullptr;

		syntactic->Putback(1);
		return eRETURN_STATE::GOOD;
	}
}