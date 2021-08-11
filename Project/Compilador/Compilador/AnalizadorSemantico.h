#pragma once
#include "AnalizadorSintactico.h"
#include "ErrorManager.h"

namespace Compilador
{
	class AnalizadorSemantico
	{
	public:
		AnalizadorSemantico(AnalizadorSintactico* syntactic, ErrorManager^ errorMan);
		~AnalizadorSemantico();

		bool Parce(std::map<std::string, StatementNode*> funcTrees);

		bool AddError(std::string errorString);

		std::string LogicTreeChecker(LogExpNode* node, std::string functionName);

		std::string CreateIntermidiateCode(LogExpNode* node, std::string functionName);

		bool SearchStatementTrees(StatementNode* node, std::string funcName);

		std::string GetIntermidiateCode();

	private:
		msclr::gcroot<ErrorManager^> m_errorManager;	
		AnalizadorSintactico* m_syntactic;

		std::map<std::string, StatementNode*> m_functionStatementsTrees;

		std::string m_intermidiateCode = "";
	};
}

