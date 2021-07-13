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
	    eRETURN_STATE r = OR(syntactic);


	
	    return eRETURN_STATE::BAD;
	}

	eRETURN_STATE Syn_LogicExpresion::OR(AnalizadorSintactico* syntactic)
	{
		return eRETURN_STATE();
	}

	eRETURN_STATE Syn_LogicExpresion::AND(AnalizadorSintactico* syntactic)
	{
		return eRETURN_STATE();
	}

	eRETURN_STATE Syn_LogicExpresion::Equality(AnalizadorSintactico* syntactic)
	{
		return eRETURN_STATE();
	}

	eRETURN_STATE Syn_LogicExpresion::Inequality(AnalizadorSintactico* syntactic)
	{
		return eRETURN_STATE();
	}
	
	eRETURN_STATE Syn_LogicExpresion::Expresion(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();
		

		if (tok.GetLexeme() == "+" || tok.GetLexeme() == "-")
		{
			eRETURN_STATE r = Term(syntactic);
		}
		else
		{
			
		}

	
		return eRETURN_STATE::BAD;
	}
	
	eRETURN_STATE Syn_LogicExpresion::Term(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();
		return eRETURN_STATE::BAD;
	}

	eRETURN_STATE Syn_LogicExpresion::Exp(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();



		return eRETURN_STATE::BAD;
	}

	eRETURN_STATE Syn_LogicExpresion::UnaryNot(AnalizadorSintactico* syntactic)
	{
		return eRETURN_STATE();
	}

	eRETURN_STATE Syn_LogicExpresion::Unary(AnalizadorSintactico* syntactic)
	{
		return eRETURN_STATE();
	}

	eRETURN_STATE Syn_LogicExpresion::Dimesion(AnalizadorSintactico* syntactic)
	{
		return eRETURN_STATE();
	}
	
	eRETURN_STATE Syn_LogicExpresion::K(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();
	
		if (tok.GetLexeme() == "(")
		{
			eRETURN_STATE r = Expresion(syntactic);
	
			if (r == eRETURN_STATE::GOOD)
			{
				tok = syntactic->GetNextToken();
	
				if (tok.GetLexeme() == ")")
				{
					return eRETURN_STATE::GOOD;
				}
				else
				{
					std::string errorMsg = "Expected ')' at logic expresion on line ";
					errorMsg.append(to_string(tok.GetLine()));
					if (!syntactic->AddError(errorMsg))
					{
						return eRETURN_STATE::FATAL;
					}
	
					while (tok.GetLexeme() != ")" && tok.GetLexeme() != ";" && tok.GetLexeme() != "," && tok.GetLexeme() != "}")
					{
						tok = syntactic->GetNextToken();
					}
					if (tok.GetLexeme() == ")")
					{
						return eRETURN_STATE::GOOD;
					}
					return eRETURN_STATE::BAD;
				}
	
			}
			else if (r == eRETURN_STATE::BAD)
			{
				return eRETURN_STATE::BAD;
			}
			else if (r == eRETURN_STATE::FATAL)
			{
				return eRETURN_STATE::FATAL;
			}
		}
		else if (tok.GetType() == eTOKEN_TYPE::INT_CONST || tok.GetType() == eTOKEN_TYPE::FLOAT_CONST)
		{
			return eRETURN_STATE::GOOD;
		}
		else if (tok.GetType() == eTOKEN_TYPE::ID)
		{
			tok = syntactic->GetNextToken();

			if (tok.GetLexeme() == "(")
			{
				SyntaxState* state = new Syn_FunctionCall();

				eRETURN_STATE r = state->Update(syntactic);

				if (r == eRETURN_STATE::FATAL)
				{
					return eRETURN_STATE::FATAL;
				}
				else if (r == eRETURN_STATE::BAD)
				{
					return eRETURN_STATE::BAD;
				}
			}
			else
			{
				return eRETURN_STATE::GOOD;
			}
		}
		else
		{
			std::string errorMsg = "Invalid token at logic expresion on line ";
			errorMsg.append(to_string(tok.GetLine()));
			if (!syntactic->AddError(errorMsg))
			{
				return eRETURN_STATE::FATAL;
			}

			while (tok.GetLexeme() != ")" && tok.GetLexeme() != ";" && tok.GetLexeme() != "," && tok.GetLexeme() != "}")
			{
				tok = syntactic->GetNextToken();
			}

			if (tok.GetLexeme() == ")")
			{
				return eRETURN_STATE::BAD;
			}
			return eRETURN_STATE::BAD;
		}
	
		return eRETURN_STATE::BAD;
	}
}