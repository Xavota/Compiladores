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
		Token tok = syntactic->GetNextToken();
		while (!(tok.GetType() == eTOKEN_TYPE::GROUPING && tok.GetLexeme() == ")"))
		{
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
					std::string errorMsg = "Identificator expected at parameter declarations on line ";
					errorMsg.append(to_string(tok.GetLine()));
					if (!syntactic->AddError(errorMsg))
					{
						return eRETURN_STATE::FATAL;
					}
					// Enter panik
				}
				
				tok = syntactic->GetNextToken();

				if (tok.GetType() == eTOKEN_TYPE::DIMENSION && tok.GetLexeme() == "[")
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

					delete state;
				}
				else
				{
					dims.push_back(0);
				}

				if (tok.GetType() == eTOKEN_TYPE::SEPARATOR && tok.GetLexeme() == ",")
				{
					tok = syntactic->GetNextToken();
					continue;
				}
				else if(tok.GetType() == eTOKEN_TYPE::SEPARATOR && tok.GetLexeme() == ":")
				{
					moreID = false;
					correct = true;
					break;
				}
				else if (tok.GetType() == eTOKEN_TYPE::SEPARATOR && tok.GetLexeme() == ";")
				{
					moreID = false;
					correct = false;

					std::string errorMsg = "Expected ':' after list of IDs at parameter declarations on line ";
					errorMsg.append(to_string(tok.GetLine()));
					if (!syntactic->AddError(errorMsg))
					{
						return eRETURN_STATE::FATAL;
					}
					break;
				}
				else
				{
					moreID = false;
					correct = false;

					std::string errorMsg = "Expected ':' after list of IDs at parameter declarations on line ";
					errorMsg.append(to_string(tok.GetLine()));
					if (!syntactic->AddError(errorMsg))
					{
						return eRETURN_STATE::FATAL;
					}

					// Enter Panik
					break;
				}

			}

			
			if (correct)
			{
				tok = syntactic->GetNextToken();

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
								return eRETURN_STATE::FATAL;
							}
						}
						else
						{
							syntactic->AddSymbol(Ids[i], "PARAMETER", dims[i], syntactic->GetContext(), tok.GetLexeme());
						}
					}
				}
			}
		}
		return eRETURN_STATE::GOOD;
	}
}