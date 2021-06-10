#include "pch.h"
#include "Token.h"

Compilador::Token::Token(int line, string lex, eTOKEN_TYPE type)
{
	m_line = line;
	m_lexeme = lex;
	m_type = type;
}

Compilador::Token::~Token()
{
}
