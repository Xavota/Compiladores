#include "pch.h"
#include "Syn_VariableLineState.h"

namespace Compilador
{
    Syn_VariableLineState::Syn_VariableLineState()
    {
    }
    
    Syn_VariableLineState::~Syn_VariableLineState()
    {
    }
    
    eRETURN_STATE Syn_VariableLineState::Update(AnalizadorSintactico* syntactic)
    {
        return eRETURN_STATE::GOOD;
    }
}