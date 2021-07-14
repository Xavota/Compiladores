#include "pch.h"
#include "Syn_Parameters.h"
#include "Syn_DecDimension.h"

namespace Compilador
{
	Syn_Parameters::Syn_Parameters()
	{
	}
	Syn_Parameters::~Syn_Parameters()
	{
	}
	eRETURN_STATE Syn_Parameters::Update(AnalizadorSintactico* syntactic)
	{
		Ids.clear();
		dims.clear();
		lines.clear();
		eRETURN_VAR r = MultiVariablesBlock(syntactic);

		if (r == eRETURN_VAR::FATAL)
		{
			return eRETURN_STATE::FATAL;
		}
		else if (r == eRETURN_VAR::BAD)
		{
			return eRETURN_STATE::BAD;
		}
		else if (r == eRETURN_VAR::FINISH)
		{
			return eRETURN_STATE::GOOD;
		}
		return eRETURN_STATE::BAD;
	}
	eRETURN_VAR Syn_Parameters::MultiVariablesBlock(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();
		while (tok.GetLexeme() != ")")
		{			
			eRETURN_VAR r = Variables(syntactic);
			if (r == eRETURN_VAR::FINISH || r == eRETURN_VAR::FATAL || r == eRETURN_VAR::BAD)
			{
				return r;
			}
		}
		return eRETURN_VAR::FINISH;
	}
	eRETURN_VAR Syn_Parameters::Variables(AnalizadorSintactico* syntactic)
	{
		syntactic->Putback(1);
		Token tok = syntactic->GetNextToken();
		while (true)
		{
			if (tok.GetType() == eTOKEN_TYPE::ID)
			{
				Ids.push_back(tok.GetLexeme());
				lines.push_back(tok.GetLine());
			}
			else
			{
				std::string errorMsg = "Identificator expected at parameter declarations on line ";
				errorMsg.append(to_string(tok.GetLine()));
				if (!syntactic->AddError(errorMsg))
				{
					return eRETURN_VAR::FATAL;
				}

				// Panik mode
				while (tok.GetLexeme() != ")" && tok.GetLexeme() != ";" && tok.GetLexeme() != "}")
				{
					tok = syntactic->GetNextToken();
				}
				if (tok.GetLexeme() == ")")
				{
					return eRETURN_VAR::FINISH;
				}
				else if (tok.GetLexeme() == "}")
				{
					return eRETURN_VAR::BAD;
				}
				else if (tok.GetLexeme() == ";")
				{
					Ids.clear();
					lines.clear();
					dims.clear();
					return eRETURN_VAR::FINISH_BLOCK;
				}

				return eRETURN_VAR::BAD;
			}

			tok = syntactic->GetNextToken();

			if (tok.GetLexeme() == "[")
			{
				int dim = 0;
				SyntaxState* state = new Syn_DecDimension(&dim);
				eRETURN_STATE r = state->Update(syntactic);
				delete state;
				if (r == eRETURN_STATE::GOOD)
				{
					dims.push_back(dim);
				}
				else if (r == eRETURN_STATE::FATAL)
				{
					return eRETURN_VAR::FATAL;
				}
				else if (r == eRETURN_STATE::BAD)
				{
					syntactic->Putback(1);

					tok = syntactic->GetNextToken();
					if (tok.GetLexeme() == ")")
					{
						return eRETURN_VAR::FINISH;
					}
					else if (tok.GetLexeme() == "}")
					{
						return eRETURN_VAR::BAD;
					}
					else if (tok.GetLexeme() == ";")
					{
						Ids.clear();
						lines.clear();
						dims.clear();

						return eRETURN_VAR::FINISH_BLOCK;
					}

					return eRETURN_VAR::BAD;
				}

			}
			else
			{
				dims.push_back(0);
			}

			if (tok.GetLexeme() == ",")
			{
				tok = syntactic->GetNextToken();
				continue;
			}
			else if (tok.GetLexeme() == ":")
			{
				return VariablesType(syntactic);
			}
			else if (tok.GetLexeme() == ";")
			{
				std::string errorMsg = "Expected ':' after list of IDs at parameter declarations on line ";
				errorMsg.append(to_string(tok.GetLine()));
				if (!syntactic->AddError(errorMsg))
				{
					return eRETURN_VAR::FATAL;
				}

				Ids.clear();
				lines.clear();
				dims.clear();

				return eRETURN_VAR::FINISH_BLOCK;
			}
			else
			{
				std::string errorMsg = "Expected ':' after list of IDs at parameter declarations on line ";
				errorMsg.append(to_string(tok.GetLine()));
				if (!syntactic->AddError(errorMsg))
				{
					return eRETURN_VAR::FATAL;
				}


				// Panik mode
				while (tok.GetLexeme() != ")" && tok.GetLexeme() != ";" && tok.GetLexeme() != "}")
				{
					tok = syntactic->GetNextToken();
				}
				if (tok.GetLexeme() == ")")
				{
					return eRETURN_VAR::FINISH;
				}
				else if (tok.GetLexeme() == "}")
				{
					return eRETURN_VAR::BAD;
				}
				else if (tok.GetLexeme() == ";")
				{
					Ids.clear();
					lines.clear();
					dims.clear();

					return eRETURN_VAR::FINISH_BLOCK;
				}

				return eRETURN_VAR::BAD;
			}
		}
		return eRETURN_VAR::BAD;
	}
	eRETURN_VAR Syn_Parameters::VariablesType(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();

		if (IsVariableType(tok))
		{
			for (int i = 0; i < Ids.size(); i++)
			{
				if (syntactic->SymbolExist(Ids[i], "PARAMETER", syntactic->GetContext()))
				{
					std::string errorMsg = "ID '";
					errorMsg.append(Ids[i]);
					errorMsg.append("' already exist in the current context. On line ");
					errorMsg.append(to_string(tok.GetLine()));
					if (!syntactic->AddError(errorMsg))
					{
						return eRETURN_VAR::FATAL;
					}
				}
				else
				{
					syntactic->AddSymbol(lines[i], Ids[i], "PARAMETER", dims[i], syntactic->GetContext(), tok.GetLexeme());
				}
			}

			return VariablesSemicolon(syntactic);
		}
		else
		{
			std::string errorMsg = "Expected varible type after ':' on line ";
			errorMsg.append(to_string(tok.GetLine()));
			if (!syntactic->AddError(errorMsg))
			{
				return eRETURN_VAR::FATAL;
			}


			// Panik mode
			while (tok.GetLexeme() != ")" && tok.GetLexeme() != ";" && tok.GetLexeme() != "}")
			{
				tok = syntactic->GetNextToken();
			}
			if (tok.GetLexeme() == ")")
			{
				return eRETURN_VAR::FINISH;
			}
			else if (tok.GetLexeme() == "}")
			{
				return eRETURN_VAR::BAD;
			}
			else if (tok.GetLexeme() == ";")
			{
				Ids.clear();
				lines.clear();
				dims.clear();

				return eRETURN_VAR::FINISH_BLOCK;
			}

			return eRETURN_VAR::BAD;
		}
		return eRETURN_VAR::BAD;	
	}
	eRETURN_VAR Syn_Parameters::VariablesSemicolon(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();
		if (tok.GetLexeme() == ";")
		{	
			return eRETURN_VAR::FINISH_BLOCK;
		}
		else
		{
			if (tok.GetLexeme() != ")")
			{				
				std::string errorMsg = "Expected ';' separating variables on line ";
				errorMsg.append(to_string(tok.GetLine()));
				if (!syntactic->AddError(errorMsg))
				{
					return eRETURN_VAR::FATAL;
				}

				// Panik mode
				while (tok.GetLexeme() != ")" && tok.GetLexeme() != ";" && tok.GetLexeme() != "}")
				{
					tok = syntactic->GetNextToken();
				}
				if (tok.GetLexeme() == ")")
				{
					return eRETURN_VAR::FINISH;
				}
				else if (tok.GetLexeme() == "}")
				{
					return eRETURN_VAR::BAD;
				}
				else if (tok.GetLexeme() == ";")
				{
					Ids.clear();
					lines.clear();
					dims.clear();

					return eRETURN_VAR::FINISH_BLOCK;
				}

				return eRETURN_VAR::BAD;
			}
			else
			{
				return eRETURN_VAR::FINISH;
			}
		}

		return eRETURN_VAR::BAD;
	}
}