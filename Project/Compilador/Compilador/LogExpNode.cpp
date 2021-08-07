#include "pch.h"
#include "LogExpNode.h"

namespace Compilador
{
	LogExpNode::LogExpNode(Token tok, int dim)
	{
		m_value = tok;
		m_dim = dim;
	}
	
	LogExpNode::~LogExpNode()
	{
		if (m_left != nullptr)
		{
			delete m_left;
			m_left = nullptr;
		}
		if (m_right != nullptr)
		{
			delete m_right;
			m_right = nullptr;
		}

		for (int i = 0; i < m_subTrees.size(); i++)
		{
			delete m_subTrees[i];
		}
		m_subTrees.clear();
	}
	void LogExpNode::AddSubTree(LogExpNode* subTree)
	{
		m_subTrees.push_back(subTree);
	}
}