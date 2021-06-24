#pragma once
#include "SintaxState.h"

namespace Compilador
{
	class Syn_FunctionsState : public SyntaxState
	{
	public:
		Syn_FunctionsState();
		~Syn_FunctionsState();

		eRETURN_STATE Update(AnalizadorSintactico* syntactic);
	private:

	};
}
