#pragma once
#include "LexicState.h"
#include <string>
using namespace std;

namespace Compilador
{
	class MLCommentState : public LexicState
	{
	public:
		MLCommentState(string buffer);
		~MLCommentState();

		LexicState* NextChar(char c, int& putback, class AnalizadorLexico* lexic);
		void EndOfFile(class AnalizadorLexico* lexic);

	private:
		bool m_inComment = false;
		bool m_readAsterisk = false;

		int m_startLine = 0;
	};
}

