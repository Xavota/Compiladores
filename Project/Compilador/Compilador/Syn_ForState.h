#pragma once
#include "SintaxState.h"

namespace Compilador
{
	class Syn_ForState : public SyntaxState
	{
	public:
		Syn_ForState(bool* hasReturn);
		~Syn_ForState();

		eRETURN_STATE Update(AnalizadorSintactico* syntactic);

	private:
		eRETURN_STATE OpenParenthesis(AnalizadorSintactico* syntactic);
		eRETURN_STATE VariableAsignation(AnalizadorSintactico* syntactic);
		eRETURN_STATE Condition(AnalizadorSintactico* syntactic);
		eRETURN_STATE Semicolon(AnalizadorSintactico* syntactic);
		eRETURN_STATE IncDecVariable(AnalizadorSintactico* syntactic);
		eRETURN_STATE GetIncDecID(AnalizadorSintactico* syntactic);
		eRETURN_STATE CloseParenthesis(AnalizadorSintactico* syntactic);
		eRETURN_STATE OpenBrackets(AnalizadorSintactico* syntactic);
		eRETURN_STATE CloseBrackets(AnalizadorSintactico* syntactic);
		
	private:
		bool* m_hasReturn;

		LogExpNode* m_subeLogExp = nullptr;

		eEXTRA_INFO m_extraInfo = eEXTRA_INFO::FOR_INC;
		Token m_incDecTok;
		int m_incDecVal = 1;
	};
}
