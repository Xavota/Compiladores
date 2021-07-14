#pragma once
#include "SintaxState.h"

namespace Compilador
{
	class Syn_MainState : public SyntaxState
	{
	public:
		Syn_MainState();
		~Syn_MainState();

		eRETURN_STATE Update(AnalizadorSintactico* syntactic);

		eRETURN_STATE OpenParenthesis(AnalizadorSintactico* syntactic);
		eRETURN_STATE CloseParenthesis(AnalizadorSintactico* syntactic);
		eRETURN_STATE OpenBrackets(AnalizadorSintactico* syntactic);
	private:
		bool m_hasReturn = false;
	};
}
