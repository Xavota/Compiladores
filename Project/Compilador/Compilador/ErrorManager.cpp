#include "pch.h"
#include "ErrorManager.h"

Compilador::ErrorManager::ErrorManager()
{
}

Compilador::ErrorManager::~ErrorManager()
{
}

bool Compilador::ErrorManager::AddError(String^ err, eERROR_TYPE type)
{
	cli::array<Error^>^ temp = gcnew cli::array<Error^>(m_errorCount);
	for (int i = 0; i < m_errorCount; i++)
	{
		temp[i] = m_errors[i];
	}

	delete[] m_errors;
	m_errors = gcnew cli::array<Error^>(++m_errorCount);

	for (int i = 0; i < m_errorCount - 1; i++)
	{
		m_errors[i] = temp[i];
	}
	m_errors[m_errorCount - 1]->m_errorString = err;
	m_errors[m_errorCount - 1]->m_type = type;

	int errorsFromType = 0;

	for (int i = 0; i < m_errorCount; i++)
	{
		if (m_errors[i]->m_type == type)
		{
			errorsFromType++;
		}
	}

	return errorsFromType < 10;
}

void Compilador::ErrorManager::Clean()
{
	m_errorCount = 0;
	delete m_errors;
	m_errors = gcnew cli::array<Error^>(0);
}
