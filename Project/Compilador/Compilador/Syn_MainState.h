#pragma once
#include "SintaxState.h"

namespace Compilador
{
	class Syn_MainState : public SyntaxState
	{
	public:
		Syn_MainState();
		~Syn_MainState();

		bool Update(AnalizadorSintactico* syntactic);
	private:

	};
}
