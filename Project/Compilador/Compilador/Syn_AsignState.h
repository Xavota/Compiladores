#pragma once
#include "SintaxState.h"

namespace Compilador
{
	class Syn_AsignState : public SyntaxState
	{
	public:
		Syn_AsignState();
		~Syn_AsignState();

		eRETURN_STATE Update(AnalizadorSintactico* syntactic);
	private:
		eRETURN_STATE Dim(AnalizadorSintactico* syntactic);
		eRETURN_STATE CloseSquareBrackets(AnalizadorSintactico* syntactic);
		eRETURN_STATE Equal(AnalizadorSintactico* syntactic);
		eRETURN_STATE SemiColon(AnalizadorSintactico* syntactic);

	private:
		LogExpNode* m_subLogExp = nullptr;
		LogExpNode* m_dimLogExp = nullptr;
		StatementNode* m_stNode = nullptr;

		Token m_tok;
	};
}

