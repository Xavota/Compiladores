#pragma once
#include "SintaxState.h"

namespace Compilador
{
	class Syn_FunctionState : public SyntaxState
	{
	public:
		Syn_FunctionState();
		~Syn_FunctionState();

		eRETURN_STATE Update(AnalizadorSintactico* syntactic);
	private:

	};
}
