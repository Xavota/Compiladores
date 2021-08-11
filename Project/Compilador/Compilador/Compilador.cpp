#include "pch.h"

#include "Compilador.h"

#include "SymbolTable.h"

Compilador::Manager::Manager()
{
	m_erroManager = gcnew ErrorManager();
	m_lexicAnalizer = new AnalizadorLexico(m_erroManager);
	m_syntacticAnalizer = new AnalizadorSintactico(m_erroManager);
	m_semanticAnalizer = new AnalizadorSemantico(m_syntacticAnalizer, m_erroManager);
}

Compilador::Manager::~Manager()
{
	
}

void CountSymbol(Compilador::Symbol* sy, int& count)
{
	if (sy != nullptr)
		count++;
	else
		return;

	CountSymbol(sy->m_next, count);
}

void CountSymbols(Compilador::SymbolTable* st, int& count)
{
	for (std::map<std::string, Compilador::Symbol*>::iterator it = st->m_symbols.begin(); it != st->m_symbols.end(); it++)
	{
		CountSymbol(it->second, count);
	}
}

void AddSymbol(Compilador::Symbol* sy, cli::array<String^>^ compRs, int& offset)
{
	if (sy == nullptr)
		return;

	string text = to_string(sy->m_line);
	text.append("\n");
	text.append(sy->m_name);
	text.append("\n");
	text.append(sy->m_cathegory);
	text.append("\n");
	text.append(to_string(sy->m_dimension));
	text.append("\n");
	text.append(sy->m_type);
	text.append("\n");
	text.append(sy->m_functionName);
	text.append("\n");

	compRs[offset++] = gcnew String(text.c_str());

	AddSymbol(sy->m_next, compRs, offset);
}

void AddSymbols(Compilador::SymbolTable* st, cli::array<String^>^ compRs, int& offset)
{
	for (std::map<std::string, Compilador::Symbol*>::iterator it = st->m_symbols.begin(); it != st->m_symbols.end(); it++)
	{
		AddSymbol(it->second, compRs, offset);
	}
}

cli::array<String^>^ Compilador::Manager::Compilar(String^ codigoFuente)
{
	m_erroManager->Clean();
	m_lexicAnalizer->Clean();

	bool completed = m_lexicAnalizer->Parce((const char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(codigoFuente).ToPointer());
	std::vector<Token> tokens = m_lexicAnalizer->GetTokens();

	if (completed)
		completed = m_syntacticAnalizer->Parce(tokens);

	if (completed)
		completed = m_semanticAnalizer->Parce(m_syntacticAnalizer->m_functionStatementsTrees);

	cli::array<Error^>^ errors = m_erroManager->GetErrors();

	int stringCount = 7;
	stringCount += tokens.size();
	stringCount += m_erroManager->GetErrorCount();
	CountSymbols(m_syntacticAnalizer->m_symbolTable, stringCount);

	int offset = 0;

	cli::array<String^>^ detallesDeCompilacion = gcnew cli::array<String^>(stringCount);

	detallesDeCompilacion[offset++] = gcnew String("@Tokens");

	for (int i = 0; i < tokens.size(); i++)
	{
		string text = to_string(tokens[i].GetLine());
		text.append("\n");
		text.append(tokens[i].GetLexeme());
		text.append("\n");
		switch (tokens[i].GetType())
		{
		case eTOKEN_TYPE::ID:
			text.append("ID");
			break;
		case eTOKEN_TYPE::INT_CONST:
			text.append("INT");
			break;
		case eTOKEN_TYPE::FLOAT_CONST:
			text.append("FLOAT");
			break;
		case eTOKEN_TYPE::STRING_CONST:
			text.append("STRING");
			break;
		case eTOKEN_TYPE::LOGIC_CONST:
			text.append("BOOL");
			break;
		case eTOKEN_TYPE::CHAR_CONST:
			text.append("CHAR");
			break;
		case eTOKEN_TYPE::KEY_WORD:
			text.append("KEY WORD");
			break;
		case eTOKEN_TYPE::RELATION_OPERATOR:
			text.append("RELATION OPERATOR");
			break;
		case eTOKEN_TYPE::ARITHMETIC_OPERATOR:
			text.append("ARITHMETIC OPERATOR");
			break;
		case eTOKEN_TYPE::LOGIC_OPERATOR:
			text.append("LOGIC OPERATOR");
			break;
		case eTOKEN_TYPE::NOT:
			text.append("NOT");
			break;
		case eTOKEN_TYPE::SEPARATOR:
			text.append("SEPARATOR");
			break;
		case eTOKEN_TYPE::GROUPING:
			text.append("GROUPING");
			break;
		case eTOKEN_TYPE::DIMENSION:
			text.append("DIMENSION");
			break;
		case eTOKEN_TYPE::ASSIGNMENT:
			text.append("ASSIGNMENT");
			break;
		}
		detallesDeCompilacion[offset++] = gcnew String(text.c_str());
	}

	detallesDeCompilacion[offset++] = gcnew String("@Symbols");

	AddSymbols(m_syntacticAnalizer->m_symbolTable, detallesDeCompilacion, offset);

	detallesDeCompilacion[offset++] = gcnew String("@Errors");

	for (int i = 0; i < m_erroManager->GetErrorCount(); i++)
	{
		detallesDeCompilacion[offset++] = errors[i]->m_errorString;
	}

	detallesDeCompilacion[offset++] = gcnew String("@Comments");
	if (completed)
		detallesDeCompilacion[offset++] = gcnew String("==================== Build complete ====================");/**/
	else
		detallesDeCompilacion[offset++] = gcnew String("Too many errors. Compilation stoped.");/**/		

	detallesDeCompilacion[offset++] = gcnew String("@InterCode");
	detallesDeCompilacion[offset++] = gcnew String(m_semanticAnalizer->GetIntermidiateCode().c_str());
	/*cli::array<String^>^ detallesDeCompilacion = gcnew cli::array<String^>(1);
	detallesDeCompilacion[0] = gcnew String("==================== Build complete ====================");/**/

	return detallesDeCompilacion;
}
