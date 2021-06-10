#include "pch.h"
#include "IDState.h"
#include <string>

#include "InitState.h"
#include "AnalizadorLexico.h"


namespace Compilador
{
IDState::IDState(string buffer)
{
	m_buffer = buffer;
}

IDState::~IDState()
{
	LexicState::~LexicState();
}

LexicState* IDState::NextChar(char c, int& putback, AnalizadorLexico* lexic)
{
	if (isLetter(c) || isDigit(c) || c == '_')
	{
		m_buffer += c;
		return this;
	}
	else
	{
		putback = 1;
		if (isLetter(m_buffer[m_buffer.size() - 1]))
		{
			if (isKeyWord(m_buffer))
			{
				lexic->AddToken(lexic->GetLine(), m_buffer, eTOKEN_TYPE::KEY_WORD);
			}
			else if (isLogicConstant(m_buffer))
			{
				lexic->AddToken(lexic->GetLine(),m_buffer, eTOKEN_TYPE::LOGIC_CONST);
			}
			else
			{
				lexic->AddToken(lexic->GetLine(), m_buffer, eTOKEN_TYPE::ID);
			}
		}
		else
		{	
			string errorMsg = "Invalid ID: '";
			errorMsg = errorMsg.append(m_buffer);
			errorMsg = errorMsg.append("' on line ");
			errorMsg = errorMsg.append(to_string(lexic->GetLine()));
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
void IDState::EndOfFile(AnalizadorLexico* lexic)
{
	if (isLetter(m_buffer[m_buffer.size() - 1]))
	{
		if (isKeyWord(m_buffer))
		{
			lexic->AddToken(lexic->GetLine(), m_buffer, eTOKEN_TYPE::KEY_WORD);
		}
		else if (isLogicConstant(m_buffer))
		{
			lexic->AddToken(lexic->GetLine(), m_buffer, eTOKEN_TYPE::LOGIC_CONST);
		}
		else
		{
			lexic->AddToken(lexic->GetLine(), m_buffer, eTOKEN_TYPE::ID);
		}
	}
	else
	{
		string errorMsg = "Invalid ID: '";
		errorMsg = errorMsg.append(m_buffer);
		errorMsg = errorMsg.append("' on line ");
		errorMsg = errorMsg.append(to_string(lexic->GetLine()));
		lexic->AddError(errorMsg);
	}
}
}