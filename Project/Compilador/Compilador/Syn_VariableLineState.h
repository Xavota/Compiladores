#pragma once
#include "SintaxState.h"

namespace Compilador
{
	class Syn_VariableLineState : public SyntaxState
	{
	public:
		Syn_VariableLineState();
		~Syn_VariableLineState();

		eRETURN_STATE Update(AnalizadorSintactico* syntactic);
	private:
	};
}
