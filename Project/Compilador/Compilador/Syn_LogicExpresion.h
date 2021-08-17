#pragma once
#include "SintaxState.h"
#include "LogExpNode.h"

namespace Compilador
{
	class Syn_LogicExpresion : public SyntaxState
	{
	public:
		Syn_LogicExpresion(LogExpNode** exit);
		~Syn_LogicExpresion();

		eRETURN_STATE Update(AnalizadorSintactico* syntactic);

	private:
		LogExpNode* OR(eRETURN_STATE& r, AnalizadorSintactico* syntactic);
		LogExpNode* AND(eRETURN_STATE& r, AnalizadorSintactico* syntactic);
		LogExpNode* Equality(eRETURN_STATE& r, AnalizadorSintactico* syntactic);
		LogExpNode* Inequality(eRETURN_STATE& r, AnalizadorSintactico* syntactic);
		LogExpNode* Expresion(eRETURN_STATE& r, AnalizadorSintactico* syntactic);
		LogExpNode* Term(eRETURN_STATE& r, AnalizadorSintactico* syntactic);
		LogExpNode* Exp(eRETURN_STATE& r, AnalizadorSintactico* syntactic);
		LogExpNode* UnaryNot(eRETURN_STATE& r, AnalizadorSintactico* syntactic);
		LogExpNode* Unary(eRETURN_STATE& r, AnalizadorSintactico* syntactic);
		LogExpNode* Dimesion(eRETURN_STATE& r, AnalizadorSintactico* syntactic);
		LogExpNode* K(eRETURN_STATE& r, AnalizadorSintactico* syntactic);

	private:
		LogExpNode** m_root;

		int m_startLine;
	};
}