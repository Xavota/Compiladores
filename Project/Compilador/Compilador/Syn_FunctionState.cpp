#include "pch.h"
#include "Syn_FunctionState.h"

#include "Syn_Parameters.h"
#include "Syn_FunctionBlock.h"

namespace Compilador
{
	Syn_FunctionState::Syn_FunctionState()
	{
	}
	Syn_FunctionState::~Syn_FunctionState()
	{
	}
	eRETURN_STATE Syn_FunctionState::Update(AnalizadorSintactico* syntactic)
	{
		m_hasReturn = false;
		eRETURN_STATE r = GetID(syntactic);
		if (r == eRETURN_STATE::FATAL)
		{
			return eRETURN_STATE::FATAL;
		}
		if (!m_hasReturn)
		{
			std::string errorMsg = "Missing return statement on function ";
			errorMsg.append(m_name);
			if (!syntactic->AddError(errorMsg))
			{
				return eRETURN_STATE::FATAL;
			}
		}
		return eRETURN_STATE::GOOD;
	} 
	eRETURN_STATE Syn_FunctionState::GetID(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();
		if (tok.GetType() == eTOKEN_TYPE::ID)
		{
			m_name = tok.GetLexeme();
			if (syntactic->SymbolExist(m_name, "FUNCTION", ""))
			{
				std::string errorMsg = "ID '";
				errorMsg.append(tok.GetLexeme());
				errorMsg.append("' already exist in the current context. On line ");
				errorMsg.append(to_string(tok.GetLine()));
				if (!syntactic->AddError(errorMsg))
				{
					return eRETURN_STATE::FATAL;
				}

				syntactic->SetContext(m_name);
			}
			else
			{
				syntactic->AddSymbol(tok.GetLine(), m_name, "FUNCTION", 0, "", "int");
				syntactic->SetContext(m_name);
			}
			return OpenParenthesis(syntactic);
		}
		else
		{
			std::string errorMsg = "Identificator expected at function declaration on line ";
			errorMsg.append(to_string(tok.GetLine()));
			if (!syntactic->AddError(errorMsg))
			{
				return eRETURN_STATE::FATAL;
			}


			// Panik mode
			while (tok.GetLexeme() != "(" && tok.GetLexeme() != ")" && tok.GetLexeme() != ";" && tok.GetLexeme() != "{" && tok.GetLexeme() != "}")
			{
				tok = syntactic->GetNextToken();
			}
			if (tok.GetLexeme() == "(")
			{
				m_name = "F";
				syntactic->AddSymbol(tok.GetLine(), m_name, "FUNCTION", 0, "", "int");
				syntactic->SetContext(m_name);

				SyntaxState* state = new Syn_Parameters();
				eRETURN_STATE r = state->Update(syntactic);
				delete state;

				if (r == eRETURN_STATE::GOOD)
				{
					return OpenBrackets(syntactic);
				}
				else if (r == eRETURN_STATE::FATAL)
				{
					syntactic->SetContext("");
					return eRETURN_STATE::FATAL;
				}
				else if (r == eRETURN_STATE::BAD)
				{
					syntactic->SetContext("");
					return eRETURN_STATE::BAD;
				}
			}
			else if (tok.GetLexeme() == ")")
			{
				m_name = "F";
				syntactic->AddSymbol(tok.GetLine(), m_name, "FUNCTION", 0, "", "int");
				syntactic->SetContext(m_name);

				return OpenBrackets(syntactic);
			}
			if (tok.GetLexeme() == "{")
			{
				m_name = "F";
				syntactic->AddSymbol(tok.GetLine(), m_name, "FUNCTION", 0, "", "int");
				syntactic->SetContext(m_name);

				SyntaxState* state = new Syn_FunctionBlock(&m_hasReturn);
				eRETURN_STATE r = state->Update(syntactic);
				delete state;
				syntactic->SetContext("");
				return r;
			}
			else if (tok.GetLexeme() == "}")
			{
				return eRETURN_STATE::GOOD;
			}
			return eRETURN_STATE::BAD;
		}
		return eRETURN_STATE::BAD;
	}
	eRETURN_STATE Syn_FunctionState::OpenParenthesis(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();
		if (tok.GetLexeme() == "(")
		{
			SyntaxState* state = new Syn_Parameters();
			eRETURN_STATE r = state->Update(syntactic);
			delete state;

			if (r == eRETURN_STATE::GOOD)
			{
				return OpenBrackets(syntactic);
			}
			else if (r == eRETURN_STATE::FATAL)
			{
				syntactic->SetContext("");
				return eRETURN_STATE::FATAL;
			}
			else if (r == eRETURN_STATE::BAD)
			{
				syntactic->SetContext("");
				return eRETURN_STATE::BAD;
			}
		}
		else
		{
			std::string errorMsg = "Expected '(' after ID of function declared on line ";
			errorMsg.append(to_string(tok.GetLine()));
			if (!syntactic->AddError(errorMsg))
			{
				syntactic->SetContext("");
				return eRETURN_STATE::FATAL;
			}

			// Panik mode
			while (tok.GetLexeme() != "(" && tok.GetLexeme() != ")" && tok.GetLexeme() != ";" && tok.GetLexeme() != "{" && tok.GetLexeme() != "}")
			{
				tok = syntactic->GetNextToken();
			}
			if (tok.GetLexeme() == "(")
			{
				SyntaxState* state = new Syn_Parameters();
				eRETURN_STATE r = state->Update(syntactic);
				delete state;

				if (r == eRETURN_STATE::GOOD)
				{
					return OpenBrackets(syntactic);
				}
				else if (r == eRETURN_STATE::FATAL)
				{
					syntactic->SetContext("");
					return eRETURN_STATE::FATAL;
				}
				else if (r == eRETURN_STATE::BAD)
				{
					syntactic->SetContext("");
					return eRETURN_STATE::BAD;
				}
			}
			else if (tok.GetLexeme() == ")")
			{
				return OpenBrackets(syntactic);
			}
			if (tok.GetLexeme() == "{")
			{
				SyntaxState* state = new Syn_FunctionBlock(&m_hasReturn);
				eRETURN_STATE r = state->Update(syntactic);
				delete state;
				syntactic->SetContext("");
				return r;
			}
			else if (tok.GetLexeme() == "}")
			{
				syntactic->SetContext("");
				return eRETURN_STATE::GOOD;
			}
			syntactic->SetContext("");
			return eRETURN_STATE::BAD;
		}
		syntactic->SetContext("");
		return eRETURN_STATE::BAD;
	}
	eRETURN_STATE Syn_FunctionState::OpenBrackets(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();
		if (tok.GetLexeme() == ":")
		{
			return Colon(syntactic);
		}
		else
		{
			std::string errorMsg = "Expected ':' after parameters of function declared on line ";
			errorMsg.append(to_string(tok.GetLine()));
			if (!syntactic->AddError(errorMsg))
			{
				syntactic->SetContext("");
				return eRETURN_STATE::FATAL;
			}

			// Panik mode
			while (tok.GetLexeme() != ";" && tok.GetLexeme() != "{" && tok.GetLexeme() != "}")
			{
				tok = syntactic->GetNextToken();
			}
			if (tok.GetLexeme() == "{")
			{
				SyntaxState* state = new Syn_FunctionBlock(&m_hasReturn);
				eRETURN_STATE r = state->Update(syntactic);
				delete state;
				syntactic->SetContext("");
				return r;
			}
			else if (tok.GetLexeme() == "}")
			{
				syntactic->SetContext("");
				return eRETURN_STATE::GOOD;
			}
			syntactic->SetContext("");
			return eRETURN_STATE::BAD;
		}
		syntactic->SetContext("");
		return eRETURN_STATE::BAD;
	}
	eRETURN_STATE Syn_FunctionState::Colon(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();
		if (IsFunctionType(tok))
		{
			syntactic->UpdateSymboltype(m_name, "FUNCTION", "", tok.GetLexeme());
			return GetType(syntactic);
		}
		else
		{
			std::string errorMsg = "Expected function type at the end of declaration function on line ";
			errorMsg.append(to_string(tok.GetLine()));
			errorMsg.append(". Asumed int");
			if (!syntactic->AddError(errorMsg))
			{
				syntactic->SetContext("");
				return eRETURN_STATE::FATAL;
			}

			// Panik mode
			while (tok.GetLexeme() != ";" && tok.GetLexeme() != "{" && tok.GetLexeme() != "}")
			{
				tok = syntactic->GetNextToken();
			}
			if (tok.GetLexeme() == "{")
			{
				SyntaxState* state = new Syn_FunctionBlock(&m_hasReturn);
				eRETURN_STATE r = state->Update(syntactic);
				delete state;
				syntactic->SetContext("");
				return r;
			}
			else if (tok.GetLexeme() == "}")
			{
				syntactic->SetContext("");
				return eRETURN_STATE::GOOD;
			}
			syntactic->SetContext("");
			return eRETURN_STATE::BAD;
		}
		syntactic->SetContext("");
		return eRETURN_STATE::BAD;
	}
	eRETURN_STATE Syn_FunctionState::GetType(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();
		if (tok.GetLexeme() == "{")
		{
			SyntaxState* state = new Syn_FunctionBlock(&m_hasReturn);
			eRETURN_STATE r = state->Update(syntactic);
			delete state;
			syntactic->SetContext("");
			return r;
		}
		else
		{
			std::string errorMsg = "Expected '{' at the start of statements of function on line ";
			errorMsg.append(to_string(tok.GetLine()));
			if (!syntactic->AddError(errorMsg))
			{
				syntactic->SetContext("");
				return eRETURN_STATE::FATAL;
			}

			// Panik mode
			while (tok.GetLexeme() != ";" && tok.GetLexeme() != "{" && tok.GetLexeme() != "}")
			{
				tok = syntactic->GetNextToken();
			}
			if (tok.GetLexeme() == "{")
			{
				SyntaxState* state = new Syn_FunctionBlock(&m_hasReturn);
				eRETURN_STATE r = state->Update(syntactic);
				delete state;
				syntactic->SetContext("");
				return r;
			}
			else if (tok.GetLexeme() == "}")
			{
				syntactic->SetContext("");
				return eRETURN_STATE::GOOD;
			}
			syntactic->SetContext("");
			return eRETURN_STATE::BAD;
		}
		syntactic->SetContext("");
		return eRETURN_STATE::BAD;
	}
}