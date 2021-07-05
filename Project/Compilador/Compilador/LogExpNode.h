#pragma once

namespace Compilador
{
	class LogExpNode
	{
	public:
		LogExpNode();
		~LogExpNode();

		

	public:
		LogExpNode* m_left;
		LogExpNode* m_right;
	};
}
