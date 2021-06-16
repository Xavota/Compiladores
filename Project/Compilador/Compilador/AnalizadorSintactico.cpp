#include "pch.h"
#include "AnalizadorSintactico.h"
#include "SintaxState.h"

#include "Syn_ProgramState.h"

namespace Compilador
{
	AnalizadorSintactico::AnalizadorSintactico(ErrorManager^ errorMan)
	{
		m_errorManager = errorMan;
	}
	AnalizadorSintactico::~AnalizadorSintactico()
	{
	}
	bool AnalizadorSintactico::Parce(std::vector<Token> tokens)
	{
		m_tokens = tokens;

		SyntaxState* state = new Syn_ProgramState();
		state->Update(this);

		return true;
	}
	Token AnalizadorSintactico::GetNextToken()
	{
		return m_tokens[m_currentTokenIndex++];
	}
	void AnalizadorSintactico::Putback(int back)
	{
		if (back < m_currentTokenIndex)
		{
			m_currentTokenIndex -= back;
		}
		else
		{
			m_currentTokenIndex = 0;
		}
	}
	bool AnalizadorSintactico::AddError(std::string errorString)
	{
		return m_errorManager->AddError(gcnew String(errorString.c_str()), eERROR_TYPE::SYNTACTIC);
	}
}