#include "pch.h"
#include "ErrorManager.h"

Compilador::ErrorManager::ErrorManager()
{
}

Compilador::ErrorManager::~ErrorManager()
{
}

bool Compilador::ErrorManager::AddError(String^ err)
{
	cli::array<String^>^ temp = gcnew cli::array<String^>(m_errorCount);
	for (int i = 0; i < m_errorCount; i++)
	{
		temp[i] = m_errors[i];
	}

	delete[] m_errors;
	m_errors = gcnew cli::array<String^>(++m_errorCount);

	for (int i = 0; i < m_errorCount - 1; i++)
	{
		m_errors[i] = temp[i];
	}
	m_errors[m_errorCount - 1] = err;

	if (m_errorCount >= 10)
	{
		return false;
	}
	return true;
}

void Compilador::ErrorManager::Clean()
{
	m_errorCount = 0;
	delete m_errors;
	m_errors = gcnew cli::array<String ^>(0);
}
