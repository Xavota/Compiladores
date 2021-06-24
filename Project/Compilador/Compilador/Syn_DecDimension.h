#pragma once
#include "SintaxState.h"

namespace Compilador
{
	class Syn_DecDimension : public SyntaxState
	{
	public:
		Syn_DecDimension(int* dim);
		~Syn_DecDimension();

		eRETURN_STATE Update(AnalizadorSintactico* syntactic);
	private:
		int* m_dim;
	};
}
