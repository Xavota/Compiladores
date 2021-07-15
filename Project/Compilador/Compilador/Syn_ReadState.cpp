#include "pch.h"
#include "Syn_ReadState.h"

namespace Compilador
{
	Syn_ReadState::Syn_ReadState()
	{
	}
	Syn_ReadState::~Syn_ReadState()
	{
	}
	eRETURN_STATE Syn_ReadState::Update(AnalizadorSintactico* syntactic)
	{
		return OpenParenthesis(syntactic);
	}
	eRETURN_STATE Syn_ReadState::OpenParenthesis(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();
		if (tok.GetLexeme() == "(")
		{
			return GetID(syntactic);
		}
		else
		{
			std::string errorMsg = "Expected '(' after read key word on line ";
			errorMsg.append(to_string(tok.GetLine()));
			if (!syntactic->AddError(errorMsg))
			{
				return eRETURN_STATE::FATAL;
			}

			//Panik Mode
			while (tok.GetLexeme() != "(" && tok.GetLexeme() != ")" && tok.GetLexeme() != ";" && tok.GetLexeme() != "}")
			{
				tok = syntactic->GetNextToken();
			}
			if (tok.GetLexeme() == "(")
			{
				return GetID(syntactic);
			}
			else if (tok.GetLexeme() == ")")
			{
				return Semicolon(syntactic);
			}
			else if (tok.GetLexeme() == ";")
			{
				return eRETURN_STATE::GOOD;
			}
			else if (tok.GetLexeme() == "}")
			{
				return eRETURN_STATE::BAD;
			}
		}
		return eRETURN_STATE::BAD;
	}
	eRETURN_STATE Syn_ReadState::GetID(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();
		if (tok.GetType() == eTOKEN_TYPE::ID)
		{
			return CloseParenthesis(syntactic);
		}
		else
		{
			std::string errorMsg = "Expected ID inside read statement on line ";
			errorMsg.append(to_string(tok.GetLine()));
			if (!syntactic->AddError(errorMsg))
			{
				return eRETURN_STATE::FATAL;
			}

			//Panik Mode
			while (tok.GetLexeme() != ")" && tok.GetLexeme() != ";" && tok.GetLexeme() != "}")
			{
				tok = syntactic->GetNextToken();
			}
			if (tok.GetLexeme() == ")")
			{
				return Semicolon(syntactic);
			}
			else if (tok.GetLexeme() == ";")
			{
				return eRETURN_STATE::GOOD;
			}
			else if (tok.GetLexeme() == "}")
			{
				return eRETURN_STATE::BAD;
			}
		}
		return eRETURN_STATE::BAD;
	}
	eRETURN_STATE Syn_ReadState::CloseParenthesis(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();
		if (tok.GetLexeme() == ")")
		{
			return Semicolon(syntactic);
		}
		else
		{
			std::string errorMsg = "Expected ')' at the end of read statement on line ";
			errorMsg.append(to_string(tok.GetLine()));
			if (!syntactic->AddError(errorMsg))
			{
				return eRETURN_STATE::FATAL;
			}

			//Panik Mode
			while (tok.GetLexeme() != ")" && tok.GetLexeme() != ";" && tok.GetLexeme() != "}")
			{
				tok = syntactic->GetNextToken();
			}
			if (tok.GetLexeme() == ")")
			{
				return Semicolon(syntactic);
			}
			else if (tok.GetLexeme() == ";")
			{
				return eRETURN_STATE::GOOD;
			}
			else if (tok.GetLexeme() == "}")
			{
				return eRETURN_STATE::BAD;
			}
		}
		return eRETURN_STATE::BAD;
	}
	eRETURN_STATE Syn_ReadState::Semicolon(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();
		if (tok.GetLexeme() == ";")
		{
			return eRETURN_STATE::GOOD;
		}
		else
		{
			std::string errorMsg = "Expected ; at the end of statement on line ";
			errorMsg.append(to_string(tok.GetLine()));
			if (!syntactic->AddError(errorMsg))
			{
				return eRETURN_STATE::FATAL;
			}

			//Panik Mode
			while (tok.GetLexeme() != ";" && tok.GetLexeme() != "}")
			{
				tok = syntactic->GetNextToken();
			}
			if (tok.GetLexeme() == ";")
			{
				return eRETURN_STATE::GOOD;
			}
			else if (tok.GetLexeme() == "}")
			{
				return eRETURN_STATE::BAD;
			}
		}
		return eRETURN_STATE::BAD;
	}
}