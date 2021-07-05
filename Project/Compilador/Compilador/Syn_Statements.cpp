#include "pch.h"
#include "Syn_Statements.h"

namespace Compilador
{
	Syn_Statements::Syn_Statements()
	{
	}
	Syn_Statements::~Syn_Statements()
	{
	}
	eRETURN_STATE Syn_Statements::Update(AnalizadorSintactico* syntactic)
	{
		return eRETURN_STATE::GOOD;
	}
}