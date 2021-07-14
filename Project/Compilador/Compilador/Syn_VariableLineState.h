#pragma once
#include "SintaxState.h"

namespace Compilador
{
	enum class eRETURN_VAR
	{
		NONE = -1,
		FINISH,
		BAD,
		FATAL,
		COUNT
	};

	class Syn_VariableLineState : public SyntaxState
	{
	public:
		Syn_VariableLineState();
		~Syn_VariableLineState();

		eRETURN_STATE Update(AnalizadorSintactico* syntactic);

	private:
		eRETURN_VAR Variables(AnalizadorSintactico* syntactic);
		eRETURN_VAR VariablesType(AnalizadorSintactico* syntactic);
		eRETURN_VAR VariablesSemicolon(AnalizadorSintactico* syntactic);

	private:
		std::vector<std::string> Ids;
		std::vector<int> dims;
		std::vector<int> lines;
	};
}
