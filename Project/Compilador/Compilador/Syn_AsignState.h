#pragma once
#include "SintaxState.h"

namespace Compilador
{
	class Syn_AsignState : public SyntaxState
	{
	public:
		Syn_AsignState();
		~Syn_AsignState();

		eRETURN_STATE Update(AnalizadorSintactico* syntactic);
	private:
		eRETURN_STATE Equal(AnalizadorSintactico* syntactic);
		eRETURN_STATE SemiColon(AnalizadorSintactico* syntactic);
	};
}

