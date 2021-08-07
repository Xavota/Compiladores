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

		void Parce(std::map<std::string, StatementNode*> funcTrees);

		bool AddError(std::string errorString);

		std::string LogicTreeChecker(LogExpNode* node, std::string functionName);

		std::string CreateIntermidiateCode(LogExpNode* node, std::string functionName);

		void SearchStatementTrees(StatementNode* node, std::string funcName);

	private:
		msclr::gcroot<ErrorManager^> m_errorManager;	
		AnalizadorSintactico* m_syntactic;

		std::vector<Token> m_tokens;
		int m_currentTokenIndex = 0;

		std::map<std::string, StatementNode*> m_functionStatementsTrees;
	};
}

