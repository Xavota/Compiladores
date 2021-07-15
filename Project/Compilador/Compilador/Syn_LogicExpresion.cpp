#include "pch.h"
#include "Syn_LogicExpresion.h"

#include "Syn_FunctionCallState.h"

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
		eRETURN_STATE r = eRETURN_STATE::GOOD;

	    m_root = OR(r, syntactic);
	
	    return r;
	}

	LogExpNode* Syn_LogicExpresion::OR(eRETURN_STATE& r, AnalizadorSintactico* syntactic)
	{
		LogExpNode* Left = AND(r, syntactic);

		while (r == eRETURN_STATE::GOOD)
		{
			Token tok = syntactic->GetNextToken();
			if (tok.GetLexeme() == "||")
			{
				LogExpNode* Parent = new LogExpNode(tok);
				Parent->m_left = Left;
				Parent->m_right = AND(r, syntactic);

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

	LogExpNode* Syn_LogicExpresion::AND(eRETURN_STATE& r, AnalizadorSintactico* syntactic)
	{
		LogExpNode* Left = Equality(r, syntactic);

		while (r == eRETURN_STATE::GOOD)
		{
			Token tok = syntactic->GetNextToken();
			if (tok.GetLexeme() == "&&")
			{
				LogExpNode* Parent = new LogExpNode(tok);
				Parent->m_left = Left;
				Parent->m_right = Equality(r, syntactic);

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

	LogExpNode* Syn_LogicExpresion::Equality(eRETURN_STATE& r, AnalizadorSintactico* syntactic)
	{
		LogExpNode* Left = Inequality(r, syntactic);

		while (r == eRETURN_STATE::GOOD)
		{
			Token tok = syntactic->GetNextToken();
			if (tok.GetLexeme() == "==")
			{
				LogExpNode* Parent = new LogExpNode(tok);
				Parent->m_left = Left;
				Parent->m_right = Inequality(r, syntactic);

				Left = Parent;
			}
			else if (tok.GetLexeme() == "!=")
			{
				LogExpNode* Parent = new LogExpNode(tok);
				Parent->m_left = Left;
				Parent->m_right = Inequality(r, syntactic);

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

	LogExpNode* Syn_LogicExpresion::Inequality(eRETURN_STATE& r, AnalizadorSintactico* syntactic)
	{
		LogExpNode* Left = Expresion(r, syntactic);

		while (r == eRETURN_STATE::GOOD)
		{
			Token tok = syntactic->GetNextToken();
			if (tok.GetLexeme() == "<")
			{
				LogExpNode* Parent = new LogExpNode(tok);
				Parent->m_left = Left;
				Parent->m_right = Expresion(r, syntactic);

				Left = Parent;
			}
			else if (tok.GetLexeme() == "<=")
			{
				LogExpNode* Parent = new LogExpNode(tok);
				Parent->m_left = Left;
				Parent->m_right = Expresion(r, syntactic);

				Left = Parent;
			}
			else if (tok.GetLexeme() == ">")
			{
				LogExpNode* Parent = new LogExpNode(tok);
				Parent->m_left = Left;
				Parent->m_right = Expresion(r, syntactic);

				Left = Parent;
			}
			else if (tok.GetLexeme() == ">=")
			{
				LogExpNode* Parent = new LogExpNode(tok);
				Parent->m_left = Left;
				Parent->m_right = Expresion(r, syntactic);

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
	
	LogExpNode* Syn_LogicExpresion::Expresion(eRETURN_STATE& r, AnalizadorSintactico* syntactic)
	{
		LogExpNode* Left = Term(r, syntactic);

		while (r == eRETURN_STATE::GOOD)
		{
			Token tok = syntactic->GetNextToken();
			if (tok.GetLexeme() == "+")
			{
				LogExpNode* Parent = new LogExpNode(tok);
				Parent->m_left = Left;
				Parent->m_right = Term(r, syntactic);

				Left = Parent;
			}
			else if (tok.GetLexeme() == "-")
			{
				LogExpNode* Parent = new LogExpNode(tok);
				Parent->m_left = Left;
				Parent->m_right = Term(r, syntactic);

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
	
	LogExpNode* Syn_LogicExpresion::Term(eRETURN_STATE& r, AnalizadorSintactico* syntactic)
	{
		LogExpNode* Left = Exp(r, syntactic);

		while (r == eRETURN_STATE::GOOD)
		{
			Token tok = syntactic->GetNextToken();
			if (tok.GetLexeme() == "*")
			{
				LogExpNode* Parent = new LogExpNode(tok);
				Parent->m_left = Left;
				Parent->m_right = Exp(r, syntactic);

				Left = Parent;
			}
			else if (tok.GetLexeme() == "/")
			{
				LogExpNode* Parent = new LogExpNode(tok);
				Parent->m_left = Left;
				Parent->m_right = Exp(r, syntactic);

				Left = Parent;
			}
			else if (tok.GetLexeme() == "%")
			{
				LogExpNode* Parent = new LogExpNode(tok);
				Parent->m_left = Left;
				Parent->m_right = Exp(r, syntactic);

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

	LogExpNode* Syn_LogicExpresion::Exp(eRETURN_STATE& r, AnalizadorSintactico* syntactic)
	{
		LogExpNode* Left = UnaryNot(r, syntactic);

		while (r == eRETURN_STATE::GOOD)
		{
			Token tok = syntactic->GetNextToken();
			if (tok.GetLexeme() == "^")
			{
				LogExpNode* Parent = new LogExpNode(tok);
				Parent->m_left = Left;
				Parent->m_right = UnaryNot(r, syntactic);
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

	LogExpNode* Syn_LogicExpresion::UnaryNot(eRETURN_STATE& r, AnalizadorSintactico* syntactic)
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
		return Unary(r, syntactic);
	}

	LogExpNode* Syn_LogicExpresion::Unary(eRETURN_STATE& r, AnalizadorSintactico* syntactic)
	{
		return Dimesion(r, syntactic);
	}

	LogExpNode* Syn_LogicExpresion::Dimesion(eRETURN_STATE& r, AnalizadorSintactico* syntactic)
	{
		return K(r, syntactic);
	}
	
	LogExpNode* Syn_LogicExpresion::K(eRETURN_STATE& r, AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();

		if (tok.GetType() == eTOKEN_TYPE::CHAR_CONST || tok.GetType() == eTOKEN_TYPE::FLOAT_CONST
		 || tok.GetType() == eTOKEN_TYPE::INT_CONST  || tok.GetType() == eTOKEN_TYPE::LOGIC_CONST || tok.GetType() == eTOKEN_TYPE::STRING_CONST)
		{
			r = eRETURN_STATE::GOOD;
			return new LogExpNode(tok);
		}
		else if (tok.GetType() == eTOKEN_TYPE::ID)
		{
			Token temp = syntactic->GetNextToken();
			if (temp.GetLexeme() == "(")
			{
				syntactic->Putback(2);
				SyntaxState* state = new Syn_FunctionCallState();
				r = state->Update(syntactic);
				if (r == eRETURN_STATE::GOOD)
				{
					return new LogExpNode(tok);
				}
				return nullptr;
			}
			else
			{	
				syntactic->Putback(1);
				return new LogExpNode(tok);
			}
		}
		else if (tok.GetLexeme() == "(")
		{
			LogExpNode* res = OR(r, syntactic);
			if (r == eRETURN_STATE::GOOD)
			{
				tok = syntactic->GetNextToken();
				if (tok.GetLexeme() == ")")
				{
					return res;
				}
				else
				{
					std::string errorMsg = "Parenthesis not closed on logic expresion on line ";
					errorMsg.append(to_string(tok.GetLine()));
					if (!syntactic->AddError(errorMsg))
					{
						r = eRETURN_STATE::FATAL;
						return nullptr;
					}

					//Panik
					while (tok.GetLexeme() != ")" && tok.GetLexeme() != "}" && tok.GetLexeme() != ";")
					{
						tok = syntactic->GetNextToken();
					}
					if (tok.GetLexeme() == ")")
					{
						r = eRETURN_STATE::GOOD;
						return res;
					}
					else if (tok.GetLexeme() != "}" || tok.GetLexeme() != ";")
					{
						r = eRETURN_STATE::BAD;
						return nullptr;
					}
					return nullptr;
				}
			}
			else if (r == eRETURN_STATE::BAD)
			{
				syntactic->Putback(1);
				tok = syntactic->GetNextToken();
				if (tok.GetLexeme() == ")")
				{
					r == eRETURN_STATE::GOOD;
					return res;
				}
				return nullptr;
			}
			else if (r == eRETURN_STATE::FATAL)
			{
				return nullptr;
			}
		}
		else
		{
			std::string errorMsg = "Invalid token on logic expresion on line ";
			errorMsg.append(to_string(tok.GetLine()));
			if (!syntactic->AddError(errorMsg))
			{
				r = eRETURN_STATE::FATAL;
				return nullptr;
			}

			//Panik
			while (tok.GetLexeme() != ")" && tok.GetLexeme() != "}" && tok.GetLexeme() != ";")
			{
				tok = syntactic->GetNextToken();
			}
			r = eRETURN_STATE::BAD;
			return nullptr;
		}
	}
}