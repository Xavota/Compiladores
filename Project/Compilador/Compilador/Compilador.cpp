#include "pch.h"

#include "Compilador.h"

Compilador::Manager::Manager()
{
	m_erroManager = gcnew ErrorManager();
	m_lexicAnalizer = new AnalizadorLexico(m_erroManager);
}

Compilador::Manager::~Manager()
{
	
}

cli::array<String^>^ Compilador::Manager::Compilar(String^ codigoFuente)
{
	m_erroManager->Clean();
	m_lexicAnalizer->Clean();

	bool completed = m_lexicAnalizer->Parce((const char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(codigoFuente).ToPointer());

	std::vector<Token> tokens = m_lexicAnalizer->GetTokens();
	cli::array<String^>^ errors = m_erroManager->GetErrors();

	int stringCount = 4;
	stringCount += tokens.size();
	stringCount += m_erroManager->GetErrorCount();

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

	detallesDeCompilacion[offset++] = gcnew String("@Errors");

	for (int i = 0; i < m_erroManager->GetErrorCount(); i++)
	{
		detallesDeCompilacion[offset++] = errors[i];
	}

	detallesDeCompilacion[offset++] = gcnew String("@Comments");
	if (completed)
		detallesDeCompilacion[offset++] = gcnew String("==================== Build complete ====================");/**/
	else
		detallesDeCompilacion[offset++] = gcnew String("Too many errors. Compilation stoped.");/**/		
	/*cli::array<String^>^ detallesDeCompilacion = gcnew cli::array<String^>(1);
	detallesDeCompilacion[0] = gcnew String("==================== Build complete ====================");/**/

	return detallesDeCompilacion;
}
