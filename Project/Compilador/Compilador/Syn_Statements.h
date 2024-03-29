#pragma once
#include "SintaxState.h"

namespace Compilador
{
	class Syn_Statements : public SyntaxState
	{
	public:
		Syn_Statements(bool* hasReturn);
		~Syn_Statements();

		eRETURN_STATE Update(AnalizadorSintactico* syntactic);
	private:
		bool* m_hasReturn;
	};
}