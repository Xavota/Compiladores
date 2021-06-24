#pragma once
#include "AnalizadorSintactico.h"

enum class eRETURN_STATE
{
	NONE = -1,
	GOOD,
	BAD,
	FATAL,
	COUNT
};

namespace Compilador
{
	class SyntaxState
	{
	public:
		SyntaxState();
		~SyntaxState();
	
		virtual eRETURN_STATE Update(AnalizadorSintactico* syntactic) = 0;

		bool IsVariableType(Token tok);
		bool IsFunctionType(Token tok);
	
	private:
		
	
	};
}
