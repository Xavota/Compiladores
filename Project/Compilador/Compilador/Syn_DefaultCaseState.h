#pragma once
#include "SintaxState.h"

namespace Compilador
{
	class Syn_DefaultCaseState : public SyntaxState
	{
	public:
		Syn_DefaultCaseState(bool* hasReturn);
		~Syn_DefaultCaseState();

		eRETURN_STATE Update(AnalizadorSintactico* syntactic);

	private:
		eRETURN_STATE Dobledot(AnalizadorSintactico* syntactic);
		eRETURN_STATE OpenBrackets(AnalizadorSintactico* syntactic);
		eRETURN_STATE CloseBrackets(AnalizadorSintactico* syntactic);

	private:
		bool* m_hasReturn;

	};
}

