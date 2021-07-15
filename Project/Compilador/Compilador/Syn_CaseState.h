#pragma once
#include "SintaxState.h"

namespace Compilador
{
	class Syn_CaseState : public SyntaxState
	{
	public:
		Syn_CaseState();
		~Syn_CaseState();

		eRETURN_STATE Update(AnalizadorSintactico* syntactic);
	private:

	};
}

