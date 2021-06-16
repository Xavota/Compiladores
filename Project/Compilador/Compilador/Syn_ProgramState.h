#pragma once
#include "SintaxState.h"

namespace Compilador
{
	class Syn_ProgramState : public SyntaxState
	{
	public:
		Syn_ProgramState();
		~Syn_ProgramState();
	
		bool Update(AnalizadorSintactico* syntactic);
	private:
		
	};
}
