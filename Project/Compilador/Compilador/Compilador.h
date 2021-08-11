#pragma once
#include "AnalizadorLexico.h"
#include "AnalizadorSintactico.h"
#include "AnalizadorSemantico.h"
#include "ErrorManager.h"

using namespace System;

namespace Compilador 
{
	public ref class Manager
	{
	public:
		Manager();
		~Manager();

		cli::array<String^>^ Compilar(String^ codigoFuente);
	private:
		AnalizadorLexico* m_lexicAnalizer;
		AnalizadorSintactico* m_syntacticAnalizer;
		AnalizadorSemantico* m_semanticAnalizer;
		ErrorManager^ m_erroManager;
	};
}
