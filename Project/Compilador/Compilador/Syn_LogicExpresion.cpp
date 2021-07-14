#include "pch.h"
#include "Syn_LogicExpresion.h"

#include "Syn_FunctionCall.h"

namespace Compilador
{
	Syn_LogicExpresion::Syn_LogicExpresion()
	{
	}
	
	Syn_LogicExpresion::~Syn_LogicExpresion()
	{
	}
	
	eRETURN_STATE Syn_LogicExpresion::Update(AnalizadorSintactico* syntactic)
	{
	   m_root = OR(syntactic);
	
	    return eRETURN_STATE::GOOD;
	}

	LogExpNode* Syn_LogicExpresion::OR(AnalizadorSintactico* syntactic)
	{
		LogExpNode* Left = AND(syntactic);

		while (true)
		{
			Token tok = syntactic->GetNextToken();
			if (tok.GetLexeme() == "||")
			{
				LogExpNode* Parent = new LogExpNode(tok);
				Parent->m_left = Left;
				Parent->m_right = AND(syntactic);

				Left = Parent;
			}
			else
			{
				syntactic->Putback(1);
				return Left;
			}
		}

		return Left;
	}

	LogExpNode* Syn_LogicExpresion::AND(AnalizadorSintactico* syntactic)
	{
		LogExpNode* Left = Equality(syntactic);

		while (true)
		{
			Token tok = syntactic->GetNextToken();
			if (tok.GetLexeme() == "&&")
			{
				LogExpNode* Parent = new LogExpNode(tok);
				Parent->m_left = Left;
				Parent->m_right = Equality(syntactic);

				Left = Parent;
			}
			else
			{
				syntactic->Putback(1);
				return Left;
			}
		}

		return Left;
	}

	LogExpNode* Syn_LogicExpresion::Equality(AnalizadorSintactico* syntactic)
	{
		LogExpNode* Left = Inequality(syntactic);

		while (true)
		{
			Token tok = syntactic->GetNextToken();
			if (tok.GetLexeme() == "==")
			{
				LogExpNode* Parent = new LogExpNode(tok);
				Parent->m_left = Left;
				Parent->m_right = Inequality(syntactic);

				Left = Parent;
			}
			else if (tok.GetLexeme() == "!=")
			{
				LogExpNode* Parent = new LogExpNode(tok);
				Parent->m_left = Left;
				Parent->m_right = Inequality(syntactic);

				Left = Parent;
			}
			else
			{
				syntactic->Putback(1);
				return Left;
			}
		}

		return Left;
	}

	LogExpNode* Syn_LogicExpresion::Inequality(AnalizadorSintactico* syntactic)
	{
		LogExpNode* Left = Expresion(syntactic);

		while (true)
		{
			Token tok = syntactic->GetNextToken();
			if (tok.GetLexeme() == "<")
			{
				LogExpNode* Parent = new LogExpNode(tok);
				Parent->m_left = Left;
				Parent->m_right = Expresion(syntactic);

				Left = Parent;
			}
			else if (tok.GetLexeme() == "<=")
			{
				LogExpNode* Parent = new LogExpNode(tok);
				Parent->m_left = Left;
				Parent->m_right = Expresion(syntactic);

				Left = Parent;
			}
			else if (tok.GetLexeme() == ">")
			{
				LogExpNode* Parent = new LogExpNode(tok);
				Parent->m_left = Left;
				Parent->m_right = Expresion(syntactic);

				Left = Parent;
			}
			else if (tok.GetLexeme() == ">=")
			{
				LogExpNode* Parent = new LogExpNode(tok);
				Parent->m_left = Left;
				Parent->m_right = Expresion(syntactic);

				Left = Parent;
			}
			else
			{
				syntactic->Putback(1);
				return Left;
			}
		}

		return Left;
	}
	
	LogExpNode* Syn_LogicExpresion::Expresion(AnalizadorSintactico* syntactic)
	{
		LogExpNode* Left = Term(syntactic);

		while (true)
		{
			Token tok = syntactic->GetNextToken();
			if (tok.GetLexeme() == "+")
			{
				LogExpNode* Parent = new LogExpNode(tok);
				Parent->m_left = Left;
				Parent->m_right = Term(syntactic);

				Left = Parent;
			}
			else if (tok.GetLexeme() == "-")
			{
				LogExpNode* Parent = new LogExpNode(tok);
				Parent->m_left = Left;
				Parent->m_right = Term(syntactic);

				Left = Parent;
			}
			else
			{
				syntactic->Putback(1);
				return Left;
			}
		}

		return Left;
	}
	
	LogExpNode* Syn_LogicExpresion::Term(AnalizadorSintactico* syntactic)
	{
		LogExpNode* Left = Exp(syntactic);

		while (true)
		{
			Token tok = syntactic->GetNextToken();
			if (tok.GetLexeme() == "*")
			{
				LogExpNode* Parent = new LogExpNode(tok);
				Parent->m_left = Left;
				Parent->m_right = Exp(syntactic);

				Left = Parent;
			}
			else if (tok.GetLexeme() == "/")
			{
				LogExpNode* Parent = new LogExpNode(tok);
				Parent->m_left = Left;
				Parent->m_right = Exp(syntactic);

				Left = Parent;
			}
			else if (tok.GetLexeme() == "%")
			{
				LogExpNode* Parent = new LogExpNode(tok);
				Parent->m_left = Left;
				Parent->m_right = Exp(syntactic);

				Left = Parent;
			}
			else
			{
				syntactic->Putback(1);
				return Left;
			}
		}

		return Left;
	}

	LogExpNode* Syn_LogicExpresion::Exp(AnalizadorSintactico* syntactic)
	{
		LogExpNode* Left = UnaryNot(syntactic);

		while (true)
		{
			Token tok = syntactic->GetNextToken();
			if (tok.GetLexeme() == "^")
			{
				LogExpNode* Parent = new LogExpNode(tok);
				Parent->m_left = Left;
				Parent->m_right = UnaryNot(syntactic);

				Left = Parent;
			}
			else
			{
				syntactic->Putback(1);
				return Left;
			}
		}

		return Left;
	}

	LogExpNode* Syn_LogicExpresion::UnaryNot(AnalizadorSintactico* syntactic)
	{
		/*while (true)
		{
			Token tok = syntactic->GetNextToken();
			if (tok.GetLexeme() == "!")
			{
				LogExpNode* Parent = new LogExpNode(tok);
				Parent->m_right = Unary(syntactic);
			}
			else
			{
				syntactic->Putback(1);
			}
		}

		return Left;*/
		return Unary(syntactic);
	}

	LogExpNode* Syn_LogicExpresion::Unary(AnalizadorSintactico* syntactic)
	{
		return Dimesion(syntactic);
	}

	LogExpNode* Syn_LogicExpresion::Dimesion(AnalizadorSintactico* syntactic)
	{
		return K(syntactic);
	}
	
	LogExpNode* Syn_LogicExpresion::K(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();

		if (tok.GetType() == eTOKEN_TYPE::CHAR_CONST || tok.GetType() == eTOKEN_TYPE::FLOAT_CONST || tok.GetType() == eTOKEN_TYPE::ID
		 || tok.GetType() == eTOKEN_TYPE::INT_CONST  || tok.GetType() == eTOKEN_TYPE::LOGIC_CONST || tok.GetType() == eTOKEN_TYPE::STRING_CONST)
		{
			return new LogExpNode(tok);
		}
		else if (tok.GetLexeme() == "(")
		{
			LogExpNode* r = OR(syntactic);
			tok = syntactic->GetNextToken();
			if (tok.GetLexeme() == ")")
			{
				return r;
			}
			else
			{
				std::string errorMsg = "Parenthesis not closed on logic expresion on line ";
				errorMsg.append(to_string(tok.GetLine()));
				syntactic->AddError(errorMsg);

				//Panik
				while (tok.GetLexeme() != ")" && tok.GetLexeme() != "}" && tok.GetLexeme() != ";")
				{
					tok = syntactic->GetNextToken();
				}
				if (tok.GetLexeme() == ")")
				{
					return r;
				}
				return nullptr;
			}
		}
	}
}