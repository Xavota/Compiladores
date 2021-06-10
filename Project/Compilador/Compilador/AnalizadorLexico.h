#pragma once
#include "windows.h"
#include "vcclr.h"
#include "ErrorManager.h"
#include "Token.h"
#include <vector>

namespace Compilador
{
class AnalizadorLexico
{
public:
	AnalizadorLexico(ErrorManager^ errorMan);
	~AnalizadorLexico();

	bool Parce(const char* sourceCode);

	void AddToken(int line, string lexema, eTOKEN_TYPE type);
	std::vector<Token> GetTokens() { return m_tokens; }

	void AddLine();
	int GetLine();

	char GetCaractersBack(int putback);

	bool AddError(string errorString);

	void Clean();
private:
	msclr::gcroot<ErrorManager^> m_errorManager;

	int m_curentLine = 1;
	int m_currentChar = 0;

	const char* m_code;

	std::vector<Token> m_tokens;	
};
}
