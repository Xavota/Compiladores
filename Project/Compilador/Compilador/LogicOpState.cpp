#include "pch.h"
#include "LogicOpState.h"
#include <string>

#include "InitState.h"
#include "AnalizadorLexico.h"

namespace Compilador
{
	LogicOpState::LogicOpState(string buffer)
	{
		m_buffer = buffer;
	}

	LogicOpState::~LogicOpState()
	{
		LexicState::~LexicState();
	}

	LexicState* LogicOpState::NextChar(char c, int& putback, AnalizadorLexico* lexic)
	{
		if (m_buffer[0] == c)
		{
			putback = 0;
			m_buffer += c;
			lexic->AddToken(lexic->GetLine(), m_buffer, eTOKEN_TYPE::LOGIC_OPERATOR);
			//delete this;
			return new InitState("");
		}
		else
		{
			putback = 1;
			string errorMsg = "Invalid operator '";
			errorMsg.append(m_buffer);
			errorMsg.append("' on line ");
			errorMsg.append(to_string(lexic->GetLine()));
			if (lexic->AddError(errorMsg))
				return new InitState("");
			else
				return nullptr;
		}
		return nullptr;
	}
	void LogicOpState::EndOfFile(AnalizadorLexico* lexic)
	{
		string errorMsg = "Invalid operator '";
		errorMsg.append(m_buffer);
		errorMsg.append("' on line ");
		errorMsg.append(to_string(lexic->GetLine()));
		lexic->AddError(errorMsg);
	}
}