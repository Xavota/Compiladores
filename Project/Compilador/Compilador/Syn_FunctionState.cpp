#include "pch.h"
#include "Syn_FunctionState.h"

namespace Compilador
{
	Syn_FunctionState::Syn_FunctionState()
	{
	}
	Syn_FunctionState::~Syn_FunctionState()
	{
	}
	eRETURN_STATE Syn_FunctionState::Update(AnalizadorSintactico* syntactic)
	{
		return eRETURN_STATE::GOOD;
	}
}