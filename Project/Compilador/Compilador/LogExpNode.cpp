#include "pch.h"
#include "LogExpNode.h"

namespace Compilador
{
	LogExpNode::LogExpNode(Token tok)
	{
		m_value = tok;
	}
	
	LogExpNode::~LogExpNode()
	{
	}
}