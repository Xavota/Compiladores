#pragma once
#include "SintaxState.h"

namespace Compilador
{
	class Syn_SwitchState : public SyntaxState
	{
	public:
		Syn_SwitchState(bool* hasReturn);
		~Syn_SwitchState();

		eRETURN_STATE Update(AnalizadorSintactico* syntactic);

	private:
		eRETURN_STATE OpenParenthesis(AnalizadorSintactico* syntactic);
		eRETURN_STATE GetID(AnalizadorSintactico* syntactic);
		eRETURN_STATE CloseParenthesis(AnalizadorSintactico* syntactic);
		eRETURN_STATE OpenBrackets(AnalizadorSintactico* syntactic);
		eRETURN_STATE Inside(AnalizadorSintactico* syntactic);

	private:
		bool* m_hasReturn;

		Token m_relatedID;
	};
}

