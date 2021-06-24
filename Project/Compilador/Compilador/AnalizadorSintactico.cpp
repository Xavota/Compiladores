#include "pch.h"
#include "AnalizadorSintactico.h"
#include "SintaxState.h"

#include "Syn_ProgramState.h"

namespace Compilador
{
	AnalizadorSintactico::AnalizadorSintactico(ErrorManager^ errorMan, SymbolTable* symbolTab)
	{
		m_errorManager = errorMan;
		m_symbolTable = symbolTab;
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

	bool AnalizadorSintactico::SymbolExist(std::string name, std::string cathegory, std::string functionName)
	{
		return m_symbolTable->SymbolExist(name, cathegory, functionName);
	}
	void AnalizadorSintactico::AddSymbol(std::string name, std::string cathegory, int dimension, std::string functionName, std::string type)
	{
		m_symbolTable->AddSymbol(name, cathegory, dimension, functionName, type);
	}
	void AnalizadorSintactico::UpdateSymboltype(std::string name, std::string cathegory, std::string functionName, std::string newType)
	{
		m_symbolTable->UpdateSymboltype(name, cathegory, functionName, newType);
	}
	void AnalizadorSintactico::SetContext(std::string con)
	{
		m_context = con;
	}
	std::string AnalizadorSintactico::GetContext()
	{
		return m_context;
	}
}