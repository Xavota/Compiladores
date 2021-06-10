#pragma once
#include "windows.h"

using namespace System;

namespace Compilador
{
ref class ErrorManager
{
public:
	ErrorManager();
	~ErrorManager();

	bool AddError(String^ err);
	cli::array<String^>^ GetErrors() { return m_errors; }
	unsigned int GetErrorCount() { return m_errorCount; }

	void Clean();

private:
	unsigned int m_errorCount = 0;
	cli::array<String^>^ m_errors;
};
}
