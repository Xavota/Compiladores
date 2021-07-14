#pragma once
#include "SintaxState.h"

namespace Compilador
{
	class Syn_ReturnState : public SyntaxState
	{
	public:
		Syn_ReturnState();
		~Syn_ReturnState();

		eRETURN_STATE Update(AnalizadorSintactico* syntactic);
	private:
	};
}

