#pragma once
#include <string>

using namespace std;

namespace Compilador
{
	enum class eTOKEN_TYPE
	{
		NONE = -1,
		ID, // Ya
		INT_CONST, // Ya
		FLOAT_CONST, // Ya
		STRING_CONST, // Ya
		LOGIC_CONST, // Ya
		CHAR_CONST, // Ya
		KEY_WORD, // Ya
		RELATION_OPERATOR, // Ya
		ARITHMETIC_OPERATOR, // Ya
		LOGIC_OPERATOR, // Ya
		NOT, // Ya
		SEPARATOR, // Ya
		GROUPING, // Ya
		DIMENSION, // Ya
		ASSIGNMENT // Ya

	};
	class Token
	{
	public:
		Token(int line, string lex, eTOKEN_TYPE type);
		~Token();

		int GetLine() { return m_line; }
		string GetLexeme() { return m_lexeme; }
		eTOKEN_TYPE GetType() { return m_type; }
	
	private:
		int m_line;
		string m_lexeme;
		eTOKEN_TYPE m_type;
	};
}
