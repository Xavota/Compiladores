#include "pch.h"
#include "Syn_VariablesState.h"
#include "Syn_VariableLineState.h"

namespace Compilador
{
	Syn_VariablesState::Syn_VariablesState()
	{
	}
	Syn_VariablesState::~Syn_VariablesState()
	{
	}
	eRETURN_STATE Syn_VariablesState::Update(AnalizadorSintactico* syntactic)
	{
		Token tok = Token(0, "", eTOKEN_TYPE::NONE);
		SyntaxState* state = nullptr;

		tok = syntactic->GetNextToken();

		while (tok.GetLexeme() == "var")
		{
			state = new Syn_VariableLineState();
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