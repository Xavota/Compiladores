#pragma once
#include "SintaxState.h"
#include "LogExpNode.h"

namespace Compilador
{
	class Syn_LogicExpresion : public SyntaxState
	{
	public:
		Syn_LogicExpresion();
		~Syn_LogicExpresion();

		eRETURN_STATE Update(AnalizadorSintactico* syntactic);

		eRETURN_STATE Expresion(AnalizadorSintactico* syntactic);
		eRETURN_STATE Term(AnalizadorSintactico* syntactic);
		eRETURN_STATE Exp(AnalizadorSintactico* syntactic);
		eRETURN_STATE K(AnalizadorSintactico* syntactic);

	private:
		LogExpNode* m_root;

		std::vector<Token> m_newOrder;
	};
}