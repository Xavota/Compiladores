#pragma once
#include "SintaxState.h"

namespace Compilador
{
	class Syn_Statements : public SyntaxState
	{
	public:
		Syn_Statements();
		~Syn_Statements();

		eRETURN_STATE Update(AnalizadorSintactico* syntactic);
	private:

	};
}