#pragma once
#include "SintaxState.h"

namespace Compilador
{
	class Syn_FunctionState : public SyntaxState
	{
	public:
		Syn_FunctionState();
		~Syn_FunctionState();

		eRETURN_STATE Update(AnalizadorSintactico* syntactic);

	private:
		eRETURN_STATE GetID(AnalizadorSintactico* syntactic);
		eRETURN_STATE OpenParenthesis(AnalizadorSintactico* syntactic);
		eRETURN_STATE OpenBrackets(AnalizadorSintactico* syntactic);
		eRETURN_STATE Colon(AnalizadorSintactico* syntactic);
		eRETURN_STATE GetType(AnalizadorSintactico* syntactic);
	private:
		std::string m_name;
		std::string m_type;

		bool m_hasReturn;
	};
}
