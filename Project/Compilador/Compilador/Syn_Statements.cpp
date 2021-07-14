#include "pch.h"
#include "Syn_Statements.h"

namespace Compilador
{
	Syn_Statements::Syn_Statements(bool* hasReturn)
	{
		m_hasReturn = hasReturn;
	}
	Syn_Statements::~Syn_Statements()
	{
	}
	eRETURN_STATE Syn_Statements::Update(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();

		while (tok.GetLexeme() != "}")
		{
			tok = syntactic->GetNextToken();
		}
		syntactic->Putback(1);
		
		*m_hasReturn = true;

		return eRETURN_STATE::GOOD;
	}
}