#include "pch.h"
#include "SintaxState.h"

namespace Compilador
{
	SyntaxState::SyntaxState()
	{
	}

	SyntaxState::~SyntaxState()
	{
	}

	bool SyntaxState::IsVariableType(Token tok)
	{
		return tok.GetType() == eTOKEN_TYPE::KEY_WORD && (tok.GetLexeme() == "int" || tok.GetLexeme() == "char" || tok.GetLexeme() == "float" || tok.GetLexeme() == "string" || tok.GetLexeme() == "bool");
	}
	bool SyntaxState::IsFunctionType(Token tok)
	{
		return tok.GetType() == eTOKEN_TYPE::KEY_WORD && (tok.GetLexeme() == "int" || tok.GetLexeme() == "char" || tok.GetLexeme() == "float" || tok.GetLexeme() == "string" || tok.GetLexeme() == "bool" || tok.GetLexeme() == "void");
	}
}