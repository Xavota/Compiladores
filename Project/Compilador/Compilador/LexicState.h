#pragma once
#include <string>
using namespace std;

namespace Compilador
{
class LexicState
{
public:
	LexicState() = default;
	LexicState(string buffer);
	virtual ~LexicState();

	virtual LexicState* NextChar(char c, int& putback, class AnalizadorLexico* lexic) = 0;
	virtual void EndOfFile(class AnalizadorLexico* lexic) = 0;

	bool isValidChar(char c);
	bool isLetter(char c);
	bool isDigit(char c);

	bool isLogicOperator(char c);
	bool isArithmeticOperator(char c);
	bool isRelationalOperator(char c);
	bool isAsigningOperator(char c);
	bool isGroupingOperator(char c);
	bool isDimensionOperator(char c);
	bool isDelimiter(char c);
	bool isOperator(char c);
	bool isWhiteSpace(char c);

	bool isKeyWord(string c);
	bool isLogicConstant(string c);

protected:
	string m_buffer;
};
}
