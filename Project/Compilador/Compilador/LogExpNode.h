#pragma once
#include "Token.h"
#include <vector>

namespace Compilador
{
	class LogExpNode
	{
	public:
		LogExpNode(Token tok, int dim);
		~LogExpNode();

		void SetDim(int dim) { m_dim = dim; }

		void AddSubTree(LogExpNode* subTree);

	public:
		LogExpNode* m_left;
		LogExpNode* m_right;

		Token m_value;
		int m_dim = 0;

		std::vector<LogExpNode*> m_subTrees;
	};
}
