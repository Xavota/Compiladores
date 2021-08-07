#pragma once


#include "LogExpNode.h"
#include "Token.h"

#include <vector>
#include <string>

namespace Compilador
{
	enum class eSTATEMENT_TYPE
	{
		NONE = -1,
		ASIGN,
		CASE,
		DIM,
		FOR,
		FUNC_CALL,
		IF,
		PRINT,
		READ,
		RETURN,
		SWITCH,
		WHILE,
		COUNT
	};

	enum class eEXTRA_INFO
	{
		NONE = -1,
		FOR_INC,
		FOR_DEC,
		ASING_DIM,
		COUNT
	};

	class StatementNode
	{
	public: 
		StatementNode(eSTATEMENT_TYPE type);
		~StatementNode();

		void AddNode(StatementNode* node);
		void AddLogicExp(LogExpNode* exp);
	
	public:
		eSTATEMENT_TYPE m_type = eSTATEMENT_TYPE::NONE;

		std::vector<StatementNode*> m_subStatements;
		std::vector<LogExpNode*> m_logExpresions;

		StatementNode* m_parent;

		Token m_relatedToken;

		eEXTRA_INFO m_extraInfo = eEXTRA_INFO::NONE;

		int m_forIncDec = 0;
	};
}
