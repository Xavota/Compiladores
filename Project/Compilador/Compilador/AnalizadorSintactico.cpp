#include "pch.h"
#include "AnalizadorSintactico.h"
#include "SintaxState.h"

#include "Syn_ProgramState.h"

namespace Compilador
{
	AnalizadorSintactico::AnalizadorSintactico(ErrorManager^ errorMan)
	{
		m_errorManager = errorMan;
		m_symbolTable = new SymbolTable();
	}
	AnalizadorSintactico::~AnalizadorSintactico()
	{
	}
	bool AnalizadorSintactico::Parce(std::vector<Token> tokens)
	{
		m_symbolTable->Clear();
		m_currentTokenIndex = 0;
		m_tokens = tokens;
		m_functionStatementsTrees.clear();

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
		std::string errorMsg = "<SYNTAX> ";
		errorMsg.append(errorString);
		return m_errorManager->AddError(gcnew String(errorMsg.c_str()), eERROR_TYPE::SYNTACTIC);
	}

	bool AnalizadorSintactico::SymbolExist(std::string name, std::string cathegory, std::string functionName)
	{
		return m_symbolTable->SymbolExist(name, cathegory, functionName);
	}
	void AnalizadorSintactico::AddSymbol(int line, std::string name, std::string cathegory, int dimension, std::string functionName, std::string type, int extraInfo)
	{
		m_symbolTable->AddSymbol(line, name, cathegory, dimension, functionName, type, extraInfo);
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
	void AnalizadorSintactico::StatementTreeStartNew(std::string functionName)
	{
		if (m_functionStatementsTrees.find(functionName) == m_functionStatementsTrees.end())
		{
			m_functionStatementsTrees[functionName] = new StatementNode(eSTATEMENT_TYPE::NONE);
		}
		m_current = m_functionStatementsTrees[functionName];
	}
	void AnalizadorSintactico::StatementTreeAddNode(StatementNode* node)
	{
		if (node != nullptr)
		{
			m_current->AddNode(node);
			m_current = node;
		}
	}
	void AnalizadorSintactico::StatementTreeReturnToParent()
	{
		if (m_current->m_parent != nullptr)
		{
			m_current = m_current->m_parent;
		}
	}
	void AnalizadorSintactico::StatementTreeAddLogicTree(LogExpNode* root)
	{
		if (m_logicTrees.find(root->m_value.GetLine()) != m_logicTrees.end())
		{
			m_logicTrees[root->m_value.GetLine()].push_back(root);
		}
		else
		{
			m_logicTrees.insert(std::make_pair(root->m_value.GetLine(), std::vector<LogExpNode*>()));
			m_logicTrees[root->m_value.GetLine()].push_back(root);
		}


		m_current->AddLogicExp(root);
	}
	std::string AnalizadorSintactico::GetSymbolType(std::string name, std::string functionName, bool& isFunction)
	{
		return m_symbolTable->GetType(name, functionName, isFunction);
	}
	std::map<int, std::string> AnalizadorSintactico::GetFuncParameterTypes(std::string functionName)
	{
		return m_symbolTable->GetFuncParameterTypes(functionName);
	}
}