#pragma once
#include "SintaxState.h"

namespace Compilador
{
	class Syn_FunctionCall : public SyntaxState
	{
	public:
		Syn_FunctionCall();
		~Syn_FunctionCall();

		eRETURN_STATE Update(AnalizadorSintactico* syntactic);
	private:

	};
}