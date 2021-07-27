#pragma once
#include "SintaxState.h"

namespace Compilador
{
	class Syn_SwitchState : public SyntaxState
	{
	public:
		Syn_SwitchState(bool* hasReturn);
		~Syn_SwitchState();

		eRETURN_STATE Update(AnalizadorSintactico* syntactic);
	private:
		bool* m_hasReturn;
	};
}

