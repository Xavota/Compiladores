#include "pch.h"
#include "StatementNode.h"

namespace Compilador
{
	StatementNode::StatementNode(eSTATEMENT_TYPE type)
	{
		m_type = type;
	}
	StatementNode::~StatementNode()
	{
		for (int i = 0; i < m_subStatements.size(); i++)
		{
			delete m_subStatements[i];
		}

		for (int i = 0; i < m_logExpresions.size(); i++)
		{
			delete m_logExpresions[i];
		}
	}
	void StatementNode::AddNode(StatementNode* node)
	{
		m_subStatements.push_back(node);
		node->m_parent = this;
	}
	void StatementNode::AddLogicExp(LogExpNode* exp)
	{
		m_logExpresions.push_back(exp);
	}
}