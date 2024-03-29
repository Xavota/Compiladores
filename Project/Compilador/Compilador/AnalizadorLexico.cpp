#include "pch.h"
#include "AnalizadorLexico.h"

#include "LexicState.h"
#include "InitState.h"

Compilador::AnalizadorLexico::AnalizadorLexico(ErrorManager^ errorMan)
{
    m_errorManager = errorMan;
}

Compilador::AnalizadorLexico::~AnalizadorLexico()
{
}

bool Compilador::AnalizadorLexico::Parce(const char* sourceCode)
{
    m_code = sourceCode;

    LexicState* lState = new InitState("");
    for (int i = 0; i < strlen(sourceCode); i++)
    {
        m_currentChar = i;

        int putback = 0;
        LexicState* lS = lState->NextChar(sourceCode[i], putback, this);
        
        //delete lState;
        lState = lS;

        i -= putback;   

        if (lState == nullptr)
        {
            return false;
        }
    }
    lState->EndOfFile(this);

    AddToken(m_curentLine, "", eTOKEN_TYPE::END);
    return true;
}

void Compilador::AnalizadorLexico::AddToken(int line, string lexema, eTOKEN_TYPE type)
{
    m_tokens.push_back(Token(line, lexema, type));
}

void Compilador::AnalizadorLexico::AddLine()
{
    m_curentLine++;
}

int Compilador::AnalizadorLexico::GetLine()
{
    return m_curentLine;
}

char Compilador::AnalizadorLexico::GetCaractersBack(int putback)
{
    if (m_currentChar < putback)
        return '\0';
    return m_code[m_currentChar - putback];
}

bool Compilador::AnalizadorLexico::AddError(string errorString)
{
	std::string errorMsg = "<LEXIC> ";
	errorMsg.append(errorString);
    return m_errorManager->AddError(gcnew String(errorMsg.c_str()), eERROR_TYPE::LEXIC);
}

void Compilador::AnalizadorLexico::Clean()
{
	m_curentLine = 1;
	m_currentChar = 0;

	m_code = nullptr;

	m_tokens.clear();
}
