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

	private:
		LogExpNode* OR(AnalizadorSintactico* syntactic);
		LogExpNode* AND(AnalizadorSintactico* syntactic);
		LogExpNode* Equality(AnalizadorSintactico* syntactic);
		LogExpNode* Inequality(AnalizadorSintactico* syntactic);
		LogExpNode* Expresion(AnalizadorSintactico* syntactic);
		LogExpNode* Term(AnalizadorSintactico* syntactic);
		LogExpNode* Exp(AnalizadorSintactico* syntactic);
		LogExpNode* UnaryNot(AnalizadorSintactico* syntactic);
		LogExpNode* Unary(AnalizadorSintactico* syntactic);
		LogExpNode* Dimesion(AnalizadorSintactico* syntactic);
		LogExpNode* K(AnalizadorSintactico* syntactic);

	private:
		LogExpNode* m_root;

		std::vector<Token> m_newOrder;
	};
}