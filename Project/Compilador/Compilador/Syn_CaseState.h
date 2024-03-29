#pragma once
#include "SintaxState.h"

namespace Compilador
{
	class Syn_CaseState : public SyntaxState
	{
	public:
		Syn_CaseState(bool* hasReturn, const Token& ID);
		~Syn_CaseState();

		eRETURN_STATE Update(AnalizadorSintactico* syntactic);

	private:
		eRETURN_STATE Constant(AnalizadorSintactico* syntactic);
		eRETURN_STATE Dobledot(AnalizadorSintactico* syntactic);
		eRETURN_STATE OpenBrackets(AnalizadorSintactico* syntactic);
		eRETURN_STATE CloseBrackets(AnalizadorSintactico* syntactic);

	private:
		bool* m_hasReturn;

		Token m_ID;
		LogExpNode* m_log = nullptr;
	};
}

