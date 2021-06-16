#pragma once
#include "SintaxState.h"

namespace Compilador
{
	class Syn_VariablesState : public SyntaxState
	{
	public:
		Syn_VariablesState();
		~Syn_VariablesState();

		bool Update(AnalizadorSintactico* syntactic);
	private:
		
	};
}
