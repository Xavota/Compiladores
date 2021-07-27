#include "pch.h"
#include "Syn_SwitchState.h"

#include "Syn_CaseState.h"

namespace Compilador
{
	Syn_SwitchState::Syn_SwitchState(bool* hasReturn)
	{
		m_hasReturn = hasReturn;
	}
	Syn_SwitchState::~Syn_SwitchState()
	{
	}
	eRETURN_STATE Syn_SwitchState::Update(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();

		SyntaxState* state = new Syn_CaseState(m_hasReturn);
		while (tok.GetLexeme() != "}")
		{
			tok = syntactic->GetNextToken();
			if (tok.GetLexeme() == "case")
			{
				state->Update(syntactic);
			}
			else if (tok.GetLexeme() == "default")
			{
				state->Update(syntactic);
			}
		}

		return eRETURN_STATE::GOOD;
	}
}