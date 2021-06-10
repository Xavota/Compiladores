#include "pch.h"
#include "LexicState.h"
#include <string>
#include "AnalizadorLexico.h"


namespace Compilador
{
LexicState::LexicState(string buffer)
{
	m_buffer = buffer;
}

LexicState::~LexicState()
{
}

bool LexicState::isValidChar(char c)
{
	return c <= 255 && c >= 0;
}

bool LexicState::isDigit(char c)
{
	return c >= '0' && c <= '9';
}

bool LexicState::isLetter(char c)
{
	return c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z';
}

bool LexicState::isLogicOperator(char c)
{
	return c == '&' || c == '|' || c == '!';
}

bool LexicState::isArithmeticOperator(char c)
{
	return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^';
}

bool LexicState::isRelationalOperator(char c)
{
	return c == '<' || c == '>' || c == '=' || c == '!';
}

bool LexicState::isAsigningOperator(char c)
{
	return c == '=';
}

bool LexicState::isGroupingOperator(char c)
{
	return c == '{' || c == '}' || c == '(' || c == ')';
}

bool LexicState::isDimensionOperator(char c)
{
	return c == '[' || c == ']';
}

bool LexicState::isDelimiter(char c)
{
	return c == ';' || c == ',' || c == ':';
}

bool LexicState::isOperator(char c)
{
	return isLogicOperator(c) || isArithmeticOperator(c) || isRelationalOperator(c) || isAsigningOperator(c);
}
bool LexicState::isWhiteSpace(char c)
{
	return false;
}
bool LexicState::isKeyWord(string c)
{
	return c == "var" || c == "int" || c == "char" || c == "float" || c == "string" || c == "bool" || c == "void" || c == "function"
		|| c == "main" || c == "for" || c == "inc" || c == "dec" || c == "while" || c == "if" || c == "else" || c == "switch"
		|| c == "default" || c == "case" || c == "return" || c == "print" || c == "read";
}
bool LexicState::isLogicConstant(string c)
{
	return c == "true" || c == "false";
}
}