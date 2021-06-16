#pragma once
#include "AnalizadorSintactico.h"

namespace Compilador
{
	class SyntaxState
	{
	public:
		SyntaxState();
		~SyntaxState();
	
		virtual bool Update(AnalizadorSintactico* syntactic) = 0;
	
	private:
		
	
	};
}
