#pragma once
#include "windows.h"
#include "vcclr.h"
#include "ErrorManager.h"
#include "SymbolTable.h"
#include "LogExpNode.h"

#include "Token.h"
#include <vector>
#include <map>

namespace Compilador
{
	class AnalizadorSintactico
	{
	public:
		AnalizadorSintactico(ErrorManager^ errorMan);
		~AnalizadorSintactico();

		bool Parce(std::vector<Token> tokens);

		Token GetNextToken();
		void Putback(int back);

		bool AddError(std::string errorString);

		bool SymbolExist(std::string name, std::string cathegory, std::string functionName);
		void AddSymbol(int line, std::string name, std::string cathegory, int dimension, std::string functionName, std::string type);

		void UpdateSymboltype(std::string name, std::string cathegory, std::string functionName, std::string newType);

		void SetContext(std::string con);
		std::string GetContext();

		void AddLogicTree(int line, LogExpNode* root);

	public:
		msclr::gcroot<ErrorManager^> m_errorManager;
		SymbolTable* m_symbolTable;

		std::vector<Token> m_tokens;
		int m_currentTokenIndex = 0;

		std::string m_context;

		std::map<int, std::vector<LogExpNode*>> m_logicTrees;
	};
}
