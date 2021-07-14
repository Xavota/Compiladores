#pragma once
#include "SintaxState.h"

namespace Compilador
{
	enum class eRETURN_VAR
	{
		NONE = -1,
		FINISH_BLOCK,
		FINISH,
		BAD,
		FATAL,
		COUNT
	};

	class Syn_Parameters : public SyntaxState
	{
	public:
		Syn_Parameters();
		~Syn_Parameters();

		eRETURN_STATE Update(AnalizadorSintactico* syntactic);

	private:
		eRETURN_VAR MultiVariablesBlock(AnalizadorSintactico* syntactic);
		eRETURN_VAR Variables(AnalizadorSintactico* syntactic);
		eRETURN_VAR VariablesType(AnalizadorSintactico* syntactic);
		eRETURN_VAR VariablesSemicolon(AnalizadorSintactico* syntactic);

	private:
		std::vector<std::string> Ids;
		std::vector<int> dims;
		std::vector<int> lines;
	};
}
