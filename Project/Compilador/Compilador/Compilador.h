#pragma once
#include "AnalizadorLexico.h"
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
		ErrorManager^ m_erroManager;
	};
}
