#pragma once
#include "windows.h"
#include "vcclr.h"
#include "ErrorManager.h"
#include "Token.h"
#include <vector>

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

	private:
		msclr::gcroot<ErrorManager^> m_errorManager;

		std::vector<Token> m_tokens;
		int m_currentTokenIndex = 0;

	};
}
