#pragma once
#include "SintaxState.h"

namespace Compilador
{
	class Syn_Parameters : public SyntaxState
	{
	public:
		Syn_Parameters();
		~Syn_Parameters();

		eRETURN_STATE Update(AnalizadorSintactico* syntactic);
	private:

	};
}
