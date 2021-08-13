#pragma once
#include "SintaxState.h"

namespace Compilador
{
	class Syn_CaseDefaultState : public SyntaxState
	{
	public:
		Syn_CaseDefaultState(bool* hasReturn, const Token& ID);
		~Syn_CaseDefaultState();

		eRETURN_STATE Update(AnalizadorSintactico* syntactic);

	private:
		eRETURN_STATE Dobledot(AnalizadorSintactico* syntactic);
		eRETURN_STATE OpenBrackets(AnalizadorSintactico* syntactic);
		eRETURN_STATE CloseBrackets(AnalizadorSintactico* syntactic);

	private:
		bool* m_hasReturn;

		Token m_ID;

	};
}