#include "pch.h"
#include "InitState.h"
#include <string>

#include "IDState.h"
#include "IntegerState.h"
#include "FloatState.h"
#include "CharState.h"
#include "StringState.h"
#include "RelationOpState.h"
#include "LogicOpState.h"
#include "MLCommentState.h"
#include "AnalizadorLexico.h"


namespace Compilador
{
InitState::InitState(string buffer)
{
	m_buffer = buffer;
}

InitState::~InitState()
{
	LexicState::~LexicState();
}

LexicState* InitState::NextChar(char c, int& putback, AnalizadorLexico* lexic)
{
	if (isLetter(c) || c == '_')
	{
		m_buffer += c;
		//delete this;
		return new IDState(m_buffer);
	}
	else if (c == '-')
	{
		m_buffer += c;
		//delete this;
		if (isLetter(lexic->GetCaractersBack(1)) || isDigit(lexic->GetCaractersBack(1)))
		{
			lexic->AddToken(lexic->GetLine(), m_buffer, eTOKEN_TYPE::ARITHMETIC_OPERATOR);
			return new InitState("");
		}
		else
		{
			return new IntegerState(m_buffer);
		}
	}
	else if (isDigit(c))
	{
		m_buffer += c;
		//delete this;
		return new IntegerState(m_buffer);
	}
	else if (c == '.')
	{
		m_buffer += c;
		//delete this;
		return new FloatState(m_buffer);
	}
	else if (c == 39)
	{
		m_buffer += c;
		//delete this;
		return new CharState(m_buffer);
	}
	else if (c == '"')
	{
		m_buffer += c;
		//delete this;
		return new StringState(m_buffer);
	}
	else if (isRelationalOperator(c))
	{
		m_buffer += c;
		//delete this;
		return new RelationOpState(m_buffer);
	}
	else if (isLogicOperator(c))
	{
		m_buffer += c;
		//delete this;
		return new LogicOpState(m_buffer);
	}
	else if (c == '/')
	{
		m_buffer += c;
		//delete this;
		return new MLCommentState(m_buffer);
	}
	else if (isArithmeticOperator(c))
	{
		m_buffer += c;
		//delete this;
		lexic->AddToken(lexic->GetLine(), m_buffer, eTOKEN_TYPE::ARITHMETIC_OPERATOR);
		return new InitState("");
	}
	else if (isDelimiter(c))
	{
		m_buffer += c;
		//delete this;
		lexic->AddToken(lexic->GetLine(), m_buffer, eTOKEN_TYPE::SEPARATOR);
		return new InitState("");
	}
	else if (isGroupingOperator(c))
	{
		m_buffer += c;
		//delete this;
		lexic->AddToken(lexic->GetLine(), m_buffer, eTOKEN_TYPE::GROUPING);
		return new InitState("");
	}
	else if (isDimensionOperator(c))
	{
		m_buffer += c;
		//delete this;
		lexic->AddToken(lexic->GetLine(), m_buffer, eTOKEN_TYPE::DIMENSION);
		return new InitState("");
	}
	else if (c == '\n')
	{
		lexic->AddLine();
		return this;
	}
	else if (c <= 32 && c >= 0)
	{
		return this;
	}
	else
	{
		m_buffer += c;
		string errorMsg = "Invalid character: '";
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
void InitState::EndOfFile(AnalizadorLexico* lexic)
{
}
}