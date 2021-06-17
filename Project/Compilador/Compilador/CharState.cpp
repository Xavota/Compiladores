#include "pch.h"
#include "CharState.h"
#include <string>

#include "InitState.h"
#include "AnalizadorLexico.h"

namespace Compilador
{
	CharState::CharState(string buffer)
	{
		m_buffer = buffer;
	}

	CharState::~CharState()
	{
		LexicState::~LexicState();
	}

	LexicState* CharState::NextChar(char c, int& putback, AnalizadorLexico* lexic)
	{
		if (c == 39)
		{
			putback = 0;
			m_buffer += c;
			if (m_buffer.size() == 3)
			{
				lexic->AddToken(lexic->GetLine(), m_buffer, eTOKEN_TYPE::CHAR_CONST);
			}
			else
			{
				string errorMsg = "Empty char constant (";
				errorMsg.append(m_buffer);
				errorMsg.append(") on line ");
				errorMsg.append(to_string(lexic->GetLine()));
				//delete this;
				if (lexic->AddError(errorMsg))
					return new InitState("");
				else
					return nullptr;
			}
			return new InitState("");
		}
		else if (c == '\n')
		{
			putback = 1;
			string errorMsg = "Char constant not closed (";
			errorMsg.append(m_buffer);
			errorMsg.append(") on line ");
			errorMsg.append(to_string(lexic->GetLine()));
			//delete this;
			if (lexic->AddError(errorMsg))
				return new InitState("");
			else
				return nullptr;
		}
		else
		{
			if (m_buffer.size() == 2)
			{
				putback = 1;
				string errorMsg = "Char constant not closed (";
				errorMsg.append(m_buffer);
				errorMsg.append(") on line ");
				errorMsg.append(to_string(lexic->GetLine()));
				//delete this;
				if (lexic->AddError(errorMsg))
					return new InitState("");
				else
					return nullptr;
			}
			m_buffer += c;
			return this;
		}
		//delete this;
		return nullptr;
	}
	void CharState::EndOfFile(AnalizadorLexico* lexic)
	{
		string errorMsg = "Char constant not closed (";
		errorMsg.append(m_buffer);
		errorMsg.append(") on line ");
		errorMsg.append(to_string(lexic->GetLine()));
		lexic->AddError(errorMsg);
	}
}