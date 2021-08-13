#pragma once
#include "windows.h"

using namespace System;

namespace Compilador
{

	enum class eERROR_TYPE
	{
		NONE = -1,
		LEXIC,
		SYNTACTIC,
		SEMANTIC,
		COUNT
	};

	ref struct Error
	{
		Error(String^ errorString, eERROR_TYPE type)
		{
			m_errorString = errorString;
			m_type = type;
		}
		String^ m_errorString;
		eERROR_TYPE m_type;
	};

ref class ErrorManager
{
public:
	ErrorManager();
	~ErrorManager();

	bool AddError(String^ err, eERROR_TYPE type);
	cli::array<Error^>^ GetErrors() { return m_errors; }
	unsigned int GetErrorCount() { return m_errorCount; }

	void Clean();

private:
	unsigned int m_errorCount = 0;
	cli::array<Error^>^ m_errors;
};
}
