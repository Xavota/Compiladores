#pragma once
#include "SintaxState.h"

namespace Compilador
{
	class Syn_SwitchState : public SyntaxState
	{
	public:
		Syn_SwitchState();
		~Syn_SwitchState();

		eRETURN_STATE Update(AnalizadorSintactico* syntactic);
	private:

	};
}

