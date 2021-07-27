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

		void Parce(std::vector<Token> m_tokens);

		bool AddError(std::string errorString);

		std::string LogicTreeChecker(LogExpNode* node, std::string functionName);

	private:
		msclr::gcroot<ErrorManager^> m_errorManager;	
		AnalizadorSintactico* m_syntactic;

		std::vector<Token> m_tokens;
		int m_currentTokenIndex = 0;
	};
}

