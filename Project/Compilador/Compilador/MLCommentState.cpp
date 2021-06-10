#include "pch.h"
#include "MLCommentState.h"
#include <string>

#include "InitState.h"
#include "AnalizadorLexico.h"

namespace Compilador
{

	MLCommentState::MLCommentState(string buffer)
	{
		m_buffer = buffer;
	}
	
	MLCommentState::~MLCommentState()
	{
		LexicState::~LexicState();
	}
	
	LexicState* MLCommentState::NextChar(char c, int& putback, AnalizadorLexico* lexic)
	{
		if (!m_inComment)
		{
			if (c == '*')
			{
				m_inComment = true;
				m_startLine = lexic->GetLine();
				return this;
			}
			else
			{
				putback = 1;
				lexic->AddToken(lexic->GetLine(), "/", eTOKEN_TYPE::ARITHMETIC_OPERATOR);
				return new InitState("");
			}
		}
		else
		{
			if (!m_readAsterisk)
			{
				if (c == '*')
				{
					m_readAsterisk = true;
				}
				else if (c == '\n')
				{
					lexic->AddLine();
				}
				return this;
			}
			else
			{
				if (c == '/')
				{
					return new InitState("");
				}
				else if (c == '\n')
				{
					m_readAsterisk = false;
					lexic->AddLine();
					return this;
				}
				else if (c == '*')
				{
					return this;
				}
				else
				{
					m_readAsterisk = false;
					return this;
				}
			}
		}
	}
	
	void MLCommentState::EndOfFile(AnalizadorLexico* lexic)
	{
		string errorMsg = "Comment not closed ";
		errorMsg.append("on line ");
		errorMsg.append(to_string(m_startLine));
		lexic->AddError(errorMsg);
	}

}