#pragma once
#include "SintaxState.h"

namespace Compilador
{
	class Syn_VariablesState : public SyntaxState
	{
	public:
		Syn_VariablesState();
		~Syn_VariablesState();

		eRETURN_STATE Update(AnalizadorSintactico* syntactic);
	private:
		
	};
}
