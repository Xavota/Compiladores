#include "pch.h"
#include "FloatState.h"
#include <string>

#include "InitState.h"
#include "AnalizadorLexico.h"


namespace Compilador
{
FloatState::FloatState(string buffer)
{
	m_buffer = buffer;
}

FloatState::~FloatState()
{
    LexicState::~LexicState();
}

LexicState* FloatState::NextChar(char c, int& putback, AnalizadorLexico* lexic)
{
    if (isDigit(c))
    {
        m_buffer += c;
        return this;
    }
    else
    {
        putback = 1;
        if (isDigit(m_buffer[m_buffer.size() - 1]))
        {
            lexic->AddToken(lexic->GetLine(), m_buffer, eTOKEN_TYPE::FLOAT_CONST);
        }
        else
        {
            string errorMsg = "Invalid float constant: '";
            errorMsg.append(m_buffer);
            errorMsg.append("' on line ");
            errorMsg.append(to_string(lexic->GetLine()));
			if (lexic->AddError(errorMsg))
				return new InitState("");
			else
				return nullptr;
        }
        //delete this;
        return new InitState("");
    }
    return nullptr;
}
void FloatState::EndOfFile(AnalizadorLexico* lexic)
{
	if (isDigit(m_buffer[m_buffer.size() - 1]))
	{
		lexic->AddToken(lexic->GetLine(), m_buffer, eTOKEN_TYPE::FLOAT_CONST);
	}
	else
	{
		string errorMsg = "Invalid float constant: '";
		errorMsg.append(m_buffer);
		errorMsg.append("' on line ");
		errorMsg.append(to_string(lexic->GetLine()));
		lexic->AddError(errorMsg);
	}
}
}