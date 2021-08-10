#pragma once
#include "SintaxState.h"

namespace Compilador
{
	class Syn_PrintState : public SyntaxState
	{
	public:
		Syn_PrintState();
		~Syn_PrintState();

		eRETURN_STATE Update(AnalizadorSintactico* syntactic);
	private:
		eRETURN_STATE OpenParenthesis(AnalizadorSintactico* syntactic);
		eRETURN_STATE Inside(AnalizadorSintactico* syntactic);
		eRETURN_STATE Semicolon(AnalizadorSintactico* syntactic);

	private:
		std::vector<LogExpNode*> m_prints;
		
	};
}

