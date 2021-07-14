#pragma once
#include "Token.h"

namespace Compilador
{
	class LogExpNode
	{
	public:
		LogExpNode(Token tok);
		~LogExpNode();

		

	public:
		LogExpNode* m_left;
		LogExpNode* m_right;

		Token m_value;
	};
}
