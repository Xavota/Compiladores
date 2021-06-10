#include "pch.h"
#include "IntegerState.h"

#include "InitState.h"
#include "FloatState.h"
#include "AnalizadorLexico.h"


namespace Compilador
{
IntegerState::IntegerState(string buffer)
{
	m_buffer = buffer;
}

IntegerState::~IntegerState()
{
	LexicState::~LexicState();
}

LexicState* IntegerState::NextChar(char c, int& putback, AnalizadorLexico* lexic)
{
    if (isDigit(c))
    {
        m_buffer += c;
        return this;
    }
    else if (c == '.')
    {
        m_buffer += c;
        //delete this;
        return new FloatState(m_buffer);
    }
    else
    {
        putback = 1;
        if (m_buffer.size() == 1 && m_buffer[0] == '-')
        {
            lexic->AddToken(lexic->GetLine(), m_buffer, eTOKEN_TYPE::ARITHMETIC_OPERATOR);
        }
        else
        {
            lexic->AddToken(lexic->GetLine(), m_buffer, eTOKEN_TYPE::INT_CONST);
        }
        //delete this;
        return new InitState("");
    }
    return nullptr;
}
void IntegerState::EndOfFile(AnalizadorLexico* lexic)
{
	if (m_buffer.size() == 1 && m_buffer[0] == '-')
	{
		lexic->AddToken(lexic->GetLine(), m_buffer, eTOKEN_TYPE::ARITHMETIC_OPERATOR);
	}
	else
	{
		lexic->AddToken(lexic->GetLine(), m_buffer, eTOKEN_TYPE::INT_CONST);
	}
}
}