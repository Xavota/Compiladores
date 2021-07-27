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
	}
	void LogExpNode::AddSubTree(LogExpNode* subTree)
	{
		m_subTrees.push_back(subTree);
	}
}