#pragma once
#include "SintaxState.h"

namespace Compilador
{
	class Syn_MainState : public SyntaxState
	{
	public:
		Syn_MainState();
		~Syn_MainState();

		eRETURN_STATE Update(AnalizadorSintactico* syntactic);
	private:

	};
}
