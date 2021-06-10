#include "pch.h"
#include "StringState.h"
#include <string>

#include "InitState.h"
#include "AnalizadorLexico.h"

namespace Compilador
{
	StringState::StringState(string buffer)
	{
		m_buffer = buffer;
	}
	
	StringState::~StringState()
	{
		LexicState::~LexicState();
	}
	
	LexicState* StringState::NextChar(char c, int& putback, AnalizadorLexico* lexic)
	{
		if (c == '"')
		{
			putback = 0;
			m_buffer += c;
			lexic->AddToken(lexic->GetLine(), m_buffer, eTOKEN_TYPE::STRING_CONST);
			return new InitState("");
		}
		else if (c == '\n')
		{
			putback = 1;
			string errorMsg = "String constant not closed (";
			errorMsg.append(m_buffer);
			errorMsg.append(") on line ");
			errorMsg.append(to_string(lexic->GetLine()));
			if (lexic->AddError(errorMsg))
				return new InitState("");
			else
				return nullptr;
		}
		else/* if (isValidChar(c))*/
		{
			m_buffer += c;
			return this;
		}
		/*else
		{
			putback = 0;
			string errorMsg = "Unrecognized character '";
			errorMsg.append(m_buffer);
			errorMsg.append("' in constant string on line ");
			errorMsg.append(to_string(lexic->GetLine()));
			lexic->AddError(errorMsg);

			return new InitState("");
		}*/
		return nullptr;
	}
	void StringState::EndOfFile(AnalizadorLexico* lexic)
	{
		string errorMsg = "String constant not closed (";
		errorMsg.append(m_buffer);
		errorMsg.append(") on line ");
		errorMsg.append(to_string(lexic->GetLine()));
		lexic->AddError(errorMsg);
	}
}