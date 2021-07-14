#pragma once
#include "SintaxState.h"

namespace Compilador
{
	class Syn_FunctionBlock : public SyntaxState
	{
	public:
		Syn_FunctionBlock(bool* hasReturn);
		~Syn_FunctionBlock();

		eRETURN_STATE Update(AnalizadorSintactico* syntactic);
	private:
		bool* m_hasReturn;
	};
}
