#pragma once
#include "SintaxState.h"

namespace Compilador
{
	class Syn_ProgramState : public SyntaxState
	{
	public:
		Syn_ProgramState();
		~Syn_ProgramState();
	
		eRETURN_STATE Update(AnalizadorSintactico* syntactic);
	private:
		
	};
}
