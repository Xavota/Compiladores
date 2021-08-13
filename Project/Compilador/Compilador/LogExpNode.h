#pragma once
#include "Token.h"
#include <vector>

namespace Compilador
{
	enum class eNODE_INFO
	{
		NONE = -1,
		DIM,
		FUNC,
		COUNT
	};

	class LogExpNode
	{
	public:
		LogExpNode(Token tok, int dim);
		~LogExpNode();

		void SetDim(int dim) { m_dim = dim; }

		void AddSubTree(LogExpNode* subTree);

	public:
		LogExpNode* m_left = nullptr;
		LogExpNode* m_right = nullptr;
			
		Token m_value;
		int m_dim = 0;

		std::vector<LogExpNode*> m_subTrees;

		eNODE_INFO m_extraInfo = eNODE_INFO::NONE;
	};
}
