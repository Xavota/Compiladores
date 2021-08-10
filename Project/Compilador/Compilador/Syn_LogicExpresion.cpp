#include "pch.h"
#include "Syn_LogicExpresion.h"

#include "Syn_FunctionCallState.h"

namespace Compilador
{
	Syn_LogicExpresion::Syn_LogicExpresion(LogExpNode** exit)
	{
		m_root = exit;
	}
	
	Syn_LogicExpresion::~Syn_LogicExpresion()
	{
	}
	
	eRETURN_STATE Syn_LogicExpresion::Update(AnalizadorSintactico* syntactic)
	{
		m_startLine = syntactic->GetNextToken().GetLine();
		syntactic->Putback(1);

		eRETURN_STATE r = eRETURN_STATE::GOOD;

	    *m_root = OR(r, syntactic);

		if (r != eRETURN_STATE::GOOD)
		{
			delete *m_root;
			*m_root = nullptr;
		}
	
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
				LogExpNode* Parent = new LogExpNode(tok, 0);
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
				LogExpNode* Parent = new LogExpNode(tok, 0);
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
				LogExpNode* Parent = new LogExpNode(tok, 0);
				Parent->m_left = Left;
				Parent->m_right = Inequality(r, syntactic);

				Left = Parent;
			}
			else if (tok.GetLexeme() == "!=")
			{
				LogExpNode* Parent = new LogExpNode(tok, 0);
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
				LogExpNode* Parent = new LogExpNode(tok, 0);
				Parent->m_left = Left;
				Parent->m_right = Expresion(r, syntactic);

				Left = Parent;
			}
			else if (tok.GetLexeme() == "<=")
			{
				LogExpNode* Parent = new LogExpNode(tok, 0);
				Parent->m_left = Left;
				Parent->m_right = Expresion(r, syntactic);

				Left = Parent;
			}
			else if (tok.GetLexeme() == ">")
			{
				LogExpNode* Parent = new LogExpNode(tok, 0);
				Parent->m_left = Left;
				Parent->m_right = Expresion(r, syntactic);

				Left = Parent;
			}
			else if (tok.GetLexeme() == ">=")
			{
				LogExpNode* Parent = new LogExpNode(tok, 0);
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
				LogExpNode* Parent = new LogExpNode(tok, 0);
				Parent->m_left = Left;
				Parent->m_right = Term(r, syntactic);

				Left = Parent;
			}
			else if (tok.GetLexeme() == "-")
			{
				LogExpNode* Parent = new LogExpNode(tok, 0);
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
				LogExpNode* Parent = new LogExpNode(tok, 0);
				Parent->m_left = Left;
				Parent->m_right = Exp(r, syntactic);

				Left = Parent;
			}
			else if (tok.GetLexeme() == "/")
			{
				LogExpNode* Parent = new LogExpNode(tok, 0);
				Parent->m_left = Left;
				Parent->m_right = Exp(r, syntactic);

				Left = Parent;
			}
			else if (tok.GetLexeme() == "%")
			{
				LogExpNode* Parent = new LogExpNode(tok, 0);
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
				LogExpNode* Parent = new LogExpNode(tok, 0);
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

	LogExpNode* Syn_LogicExpresion::UnaryNot(eRETURN_STATE& r, AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();
		if (tok.GetLexeme() == "!")
		{
			LogExpNode* Parent = new LogExpNode(tok, 0);
			Parent->m_right = OR(r, syntactic);
		}
		else
		{
			syntactic->Putback(1);
			return Unary(r, syntactic);
		}
	}

	LogExpNode* Syn_LogicExpresion::Unary(eRETURN_STATE& r, AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();
		if (tok.GetType() == eTOKEN_TYPE::UNARY)
		{
			LogExpNode* Parent = new LogExpNode(tok, 0);
			Parent->m_right = OR(r, syntactic);
		}
		else
		{
			syntactic->Putback(1);
			return Dimesion(r, syntactic);
		}
	}

	LogExpNode* Syn_LogicExpresion::Dimesion(eRETURN_STATE& r, AnalizadorSintactico* syntactic)
	{
		LogExpNode* Left = K(r, syntactic);
		if (Left != nullptr && Left->m_value.GetType() == eTOKEN_TYPE::ID)
		{
			Token tok = syntactic->GetNextToken();
			if (tok.GetLexeme() == "[")
			{
				LogExpNode* result = nullptr;
				SyntaxState* state = new Syn_LogicExpresion(&result);
				r = state->Update(syntactic);
				if (r == eRETURN_STATE::GOOD)
				{
					tok = syntactic->GetNextToken();
					if (tok.GetLexeme() == "]")
					{
						Left->SetDim(tok.GetLine());
						Left->m_extraInfo = eNODE_INFO::DIM;
						Left->AddSubTree(result);
						return Left;
					}
					else
					{
						std::string errorMsg = 
						                "Expected '[' after logic expresion on dimension on line ";
						errorMsg.append(to_string(tok.GetLine()));
						if (!syntactic->AddError(errorMsg))
						{
							r = eRETURN_STATE::FATAL;
							return nullptr;
						}

						//Panik
						while (tok.GetLexeme() != ")" && tok.GetLexeme() != "}" 
						    && tok.GetLexeme() != ";" && tok.GetLexeme() != "]"
							&& tok.GetType() != eTOKEN_TYPE::END)
						{
							tok = syntactic->GetNextToken();
						}
						if (tok.GetLexeme() == "]")
						{
							Left->SetDim(tok.GetLine());
							Left->m_extraInfo = eNODE_INFO::DIM;
							Left->AddSubTree(result);
							return Left;
						}
						else if (tok.GetLexeme() == "}" || tok.GetLexeme() == ";"
							  || tok.GetLexeme() == ")" || tok.GetType() == eTOKEN_TYPE::END)
						{
							r = eRETURN_STATE::BAD;
							return nullptr;
						}
						r = eRETURN_STATE::BAD;
						return nullptr;
					}
				}
				else if (r == eRETURN_STATE::BAD)
				{
					syntactic->Putback(1);
					tok = syntactic->GetNextToken();

					if (tok.GetLexeme() == "]")
					{
						Left->m_extraInfo = eNODE_INFO::DIM;
						Left->AddSubTree(new LogExpNode(Token(tok.GetLine(), "0", 
						                                              eTOKEN_TYPE::INT_CONST), 0));
						r = eRETURN_STATE::GOOD;
						return Left;
					}
					else if (tok.GetLexeme() == "}" || tok.GetLexeme() == ";"
						|| tok.GetLexeme() == ")" || tok.GetType() == eTOKEN_TYPE::END)
					{
						r = eRETURN_STATE::BAD;
						return nullptr;
					}
					r = eRETURN_STATE::BAD;
					return nullptr;
				}
				else if (r == eRETURN_STATE::FATAL)
				{
					r = eRETURN_STATE::FATAL;
					return nullptr;
				}
				r = eRETURN_STATE::BAD;
				return nullptr;
			}
			else
			{
				syntactic->Putback(1);
				return Left;
			}
		}
		else
		{
			return Left;
		}
	}
	
	LogExpNode* Syn_LogicExpresion::K(eRETURN_STATE& r, AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();

		if (tok.GetType() == eTOKEN_TYPE::CHAR_CONST || tok.GetType() == eTOKEN_TYPE::FLOAT_CONST
		 || tok.GetType() == eTOKEN_TYPE::INT_CONST  || tok.GetType() == eTOKEN_TYPE::LOGIC_CONST 
		 || tok.GetType() == eTOKEN_TYPE::STRING_CONST)
		{
			r = eRETURN_STATE::GOOD;
			return new LogExpNode(tok, 0);
		}
		else if (tok.GetType() == eTOKEN_TYPE::ID)
		{
			Token temp = syntactic->GetNextToken();
			if (temp.GetLexeme() == "(")
			{
				syntactic->Putback(2);
				std::vector<LogExpNode*> params;
				SyntaxState* state = new Syn_FunctionCallState(&params);
				r = state->Update(syntactic);
				if (r == eRETURN_STATE::GOOD)
				{
					LogExpNode* res = new LogExpNode(tok, 0);
					res->m_extraInfo = eNODE_INFO::FUNC;
					for (int i = 0; i < params.size(); i++)
					{
						res->AddSubTree(params[i]);
					}
					return res;
				}
				else if (r == eRETURN_STATE::BAD)
				{
					//"}", ";", END 
					return new LogExpNode(tok, 0);
				}
				else if (r == eRETURN_STATE::FATAL)
				{
					return nullptr;
				}
				return new LogExpNode(tok, 0);
			}
			else
			{	
				syntactic->Putback(1);
				return new LogExpNode(tok, 0);
			}
			return new LogExpNode(tok, 0);
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

					//Panik mode
					while (tok.GetLexeme() != ")" && tok.GetLexeme() != "}" 
					    && tok.GetLexeme() != ";" && tok.GetLexeme() != "]"
						&& tok.GetType() != eTOKEN_TYPE::END)
					{
						tok = syntactic->GetNextToken();
					}
					if (tok.GetLexeme() == ")")
					{
						r = eRETURN_STATE::GOOD;
						return res;
					}
					else if (tok.GetLexeme() == "}" || tok.GetLexeme() == ";" 
					      || tok.GetLexeme() == "]" || tok.GetType() == eTOKEN_TYPE::END)
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
				r = eRETURN_STATE::BAD;
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
			while (tok.GetLexeme() != ")" && tok.GetLexeme() != "}" && tok.GetLexeme() != ";" 
				&& tok.GetLexeme() != "]" && tok.GetType() != eTOKEN_TYPE::END)
			{
				tok = syntactic->GetNextToken();
			}
			r = eRETURN_STATE::BAD;
			return nullptr;
		}
	}
}