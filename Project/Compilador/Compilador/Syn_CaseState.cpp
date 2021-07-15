#include "pch.h"
#include "Syn_CaseState.h"

namespace Compilador
{
	Syn_CaseState::Syn_CaseState()
	{
	}
	Syn_CaseState::~Syn_CaseState()
	{
	}
	eRETURN_STATE Syn_CaseState::Update(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();

		while (tok.GetLexeme() != "}")
		{
			tok = syntactic->GetNextToken();
		}

		return eRETURN_STATE::GOOD;
	}
}