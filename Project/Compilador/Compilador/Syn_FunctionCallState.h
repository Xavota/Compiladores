#pragma once
#include "SintaxState.h"

namespace Compilador
{
	class Syn_FunctionCallState : public SyntaxState
	{
	public:
		Syn_FunctionCallState(std::vector<LogExpNode*>* params);
		~Syn_FunctionCallState();

		eRETURN_STATE Update(AnalizadorSintactico* syntactic);
	private:
		eRETURN_STATE OpenParenthesis(AnalizadorSintactico* syntactic);

		std::vector<LogExpNode*>* m_params;
	};
}

