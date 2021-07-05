#include "pch.h"
#include "Syn_VariableLineState.h"

#include "Syn_DecDimension.h"

namespace Compilador
{
    Syn_VariableLineState::Syn_VariableLineState()
    {
    }
    
    Syn_VariableLineState::~Syn_VariableLineState()
    {
    }
    
    eRETURN_STATE Syn_VariableLineState::Update(AnalizadorSintactico* syntactic)
	{
		Token tok = syntactic->GetNextToken();
		bool moreID = true;
		bool correct = false;
		std::vector<std::string> Ids;
		std::vector<int> dims;
		while (moreID)
		{
			if (tok.GetType() == eTOKEN_TYPE::ID)
			{
				Ids.push_back(tok.GetLexeme());
			}
			else
			{
				std::string errorMsg = "Identificator expected at variables declarations on line ";
				errorMsg.append(to_string(tok.GetLine()));
				if (!syntactic->AddError(errorMsg))
				{
					return eRETURN_STATE::FATAL;
				}

				// Panik mode
				while (tok.GetLexeme() != ")" && tok.GetLexeme() != ";" && tok.GetLexeme() != "," && tok.GetLexeme() != "}")
				{
					tok = syntactic->GetNextToken();
				}
				moreID = false;
				correct = false;
				break;
			}

			tok = syntactic->GetNextToken();

			if (tok.GetLexeme() == "[")
			{
				int dim = 0;
				SyntaxState* state = new Syn_DecDimension(&dim);
				eRETURN_STATE r = state->Update(syntactic);
				if (r == eRETURN_STATE::FATAL)
				{
					delete state;
					return eRETURN_STATE::FATAL;
				}
				else if (r == eRETURN_STATE::GOOD)
				{
					dims.push_back(dim);
				}
				else if (r == eRETURN_STATE::BAD)
				{
					moreID = false;
					correct = false;
					delete state;
					break;
				}

				delete state;
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
				moreID = false;
				correct = true;
				break;
			}
			else if (tok.GetLexeme() == ";")
			{
				std::string errorMsg = "Expected ':' after list of IDs at variables declarations on line ";
				errorMsg.append(to_string(tok.GetLine()));
				if (!syntactic->AddError(errorMsg))
				{
					return eRETURN_STATE::FATAL;
				}

				moreID = false;
				correct = false;
				break;
			}
			else
			{
				std::string errorMsg = "Expected ':' after list of IDs at variables declarations on line ";
				errorMsg.append(to_string(tok.GetLine()));
				if (!syntactic->AddError(errorMsg))
				{
					return eRETURN_STATE::FATAL;
				}


				// Panik mode
				while (tok.GetLexeme() != ")" && tok.GetLexeme() != ";" && tok.GetLexeme() != "," && tok.GetLexeme() != "}")
				{
					tok = syntactic->GetNextToken();
				}
				moreID = false;
				correct = false;
				break;
			}

		}


		if (correct)
		{
			tok = syntactic->GetNextToken();

			if (IsVariableType(tok))
			{
				std::string Context = syntactic->GetContext() == "" ? "GLOBAL" : "LOCAL";

				for (int i = 0; i < Ids.size(); i++)
				{
					if (syntactic->SymbolExist(Ids[i], Context, syntactic->GetContext()))
					{
						std::string errorMsg = "ID '";
						errorMsg.append(Ids[i]);
						errorMsg.append("' already exist in the current context. On line ");
						errorMsg.append(to_string(tok.GetLine()));
						if (!syntactic->AddError(errorMsg))
						{
							return eRETURN_STATE::FATAL;
						}
					}
					else
					{
						syntactic->AddSymbol(Ids[i], Context, dims[i], syntactic->GetContext(), tok.GetLexeme());
					}
				}

				tok = syntactic->GetNextToken();
			}
			else
			{
				std::string errorMsg = "Expected varible type after ':' on line ";
				errorMsg.append(to_string(tok.GetLine()));
				if (!syntactic->AddError(errorMsg))
				{
					return eRETURN_STATE::FATAL;
				}


				// Panik mode
				while (tok.GetLexeme() != ")" && tok.GetLexeme() != ";" && tok.GetLexeme() != "," && tok.GetLexeme() != "}")
				{
					tok = syntactic->GetNextToken();
				}
				return eRETURN_STATE::BAD;
			}
		}


		if (tok.GetLexeme() == ";")
		{
			return eRETURN_STATE::GOOD;
		}
		else
		{
			std::string errorMsg = "Expected ';' on line ";
			errorMsg.append(to_string(tok.GetLine()));
			if (!syntactic->AddError(errorMsg))
			{
				return eRETURN_STATE::FATAL;
			}


			// Panik mode
			while (tok.GetLexeme() != ")" && tok.GetLexeme() != ";" && tok.GetLexeme() != "," && tok.GetLexeme() != "}")
			{
				tok = syntactic->GetNextToken();
			}
			return eRETURN_STATE::BAD;
		}

		return eRETURN_STATE::GOOD;
    }
}