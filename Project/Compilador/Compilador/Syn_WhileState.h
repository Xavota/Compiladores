#pragma once
#include "SintaxState.h"

namespace Compilador
{
	class Syn_WhileState : public SyntaxState
	{
	public:
		Syn_WhileState(bool* hasReturn);
		~Syn_WhileState();

		eRETURN_STATE Update(AnalizadorSintactico* syntactic);

	private:
		eRETURN_STATE OpenParenthesis(AnalizadorSintactico* syntactic);		
		eRETURN_STATE CloseParenthesis(AnalizadorSintactico* syntactic);		
		eRETURN_STATE OpenBrackets(AnalizadorSintactico* syntactic);		
		eRETURN_STATE CloseBrackets(AnalizadorSintactico* syntactic);		
		
	private:
		bool* m_hasReturn;
	};
}

