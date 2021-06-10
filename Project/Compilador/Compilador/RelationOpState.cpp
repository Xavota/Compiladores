#include "pch.h"
#include "RelationOpState.h"
#include <string>

#include "InitState.h"
#include "LogicOpState.h"
#include "AnalizadorLexico.h"

namespace Compilador
{
	RelationOpState::RelationOpState(string buffer)
	{
		m_buffer = buffer;
	}

	RelationOpState::~RelationOpState()
	{
		LexicState::~LexicState();
	}

	LexicState* RelationOpState::NextChar(char c, int& putback, AnalizadorLexico* lexic)
	{
		if (c == '=')
		{
			putback = 0;
			m_buffer += c;
			lexic->AddToken(lexic->GetLine(), m_buffer, eTOKEN_TYPE::RELATION_OPERATOR);
			return new InitState("");
		}
		else
		{
			putback = 1;
			if (m_buffer[0] == '<' || m_buffer[0] == '>')
			{
				lexic->AddToken(lexic->GetLine(), m_buffer, eTOKEN_TYPE::RELATION_OPERATOR);
				return new InitState("");
			}
			else if (m_buffer[0] == '!')
			{
				lexic->AddToken(lexic->GetLine(), m_buffer, eTOKEN_TYPE::NOT);
				return new InitState("");
			}
			else if (m_buffer[0] == '=')
			{
				lexic->AddToken(lexic->GetLine(), m_buffer, eTOKEN_TYPE::ASSIGNMENT);
				return new InitState("");
			}
		}
		return nullptr;
	}
	void RelationOpState::EndOfFile(AnalizadorLexico* lexic)
	{
		if (m_buffer[0] == '<' || m_buffer[0] == '>')
		{
			lexic->AddToken(lexic->GetLine(), m_buffer, eTOKEN_TYPE::RELATION_OPERATOR);
		}
		else if (m_buffer[0] == '!')
		{
			lexic->AddToken(lexic->GetLine(), m_buffer, eTOKEN_TYPE::NOT);
		}
		else if (m_buffer[0] == '=')
		{
			lexic->AddToken(lexic->GetLine(), m_buffer, eTOKEN_TYPE::ASSIGNMENT);
		}
	}
}