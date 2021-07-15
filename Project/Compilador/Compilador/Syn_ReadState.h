#pragma once
#include "SintaxState.h"

namespace Compilador
{
	class Syn_ReadState : public SyntaxState
	{
	public:
		Syn_ReadState();
		~Syn_ReadState();

		eRETURN_STATE Update(AnalizadorSintactico* syntactic);

	private:
		eRETURN_STATE OpenParenthesis(AnalizadorSintactico* syntactic);
		eRETURN_STATE GetID(AnalizadorSintactico* syntactic);
		eRETURN_STATE CloseParenthesis(AnalizadorSintactico* syntactic);
		eRETURN_STATE Semicolon(AnalizadorSintactico* syntactic);

	private:
	};
}

