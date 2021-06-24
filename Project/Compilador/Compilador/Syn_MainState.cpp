#include "pch.h"
#include "Syn_MainState.h"

namespace Compilador
{
	Syn_MainState::Syn_MainState()
	{
	}
	Syn_MainState::~Syn_MainState()
	{
	}
	eRETURN_STATE Syn_MainState::Update(AnalizadorSintactico* syntactic)
	{
		return eRETURN_STATE::GOOD;
	}
}
