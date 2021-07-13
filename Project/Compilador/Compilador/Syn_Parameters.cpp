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
		while (tok.GetLexeme() != ")")
		{
			bool moreID = true;
			bool correct = false;
			std::vector<std::string> Ids;
			std::vector<int> dims;
			std::vector<int> lines;
			while (moreID)
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
				else if(tok.GetLexeme() == ":")
				{
					moreID = false;
					correct = true;
					break;
				}
				else if (tok.GetLexeme() == ";")
				{
					std::string errorMsg = "Expected ':' after list of IDs at parameter declarations on line ";
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
					std::string errorMsg = "Expected ':' after list of IDs at parameter declarations on line ";
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
							syntactic->AddSymbol(lines[i], Ids[i], "PARAMETER", dims[i], syntactic->GetContext(), tok.GetLexeme());
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
					continue;
				}
			}

			if (tok.GetLexeme() == ")")
			{
				return eRETURN_STATE::GOOD;
			}
			else
			{
				if (tok.GetLexeme() == ";")
				{
					tok = syntactic->GetNextToken();

					continue;
				}
				else
				{
					std::string errorMsg = "Expected ';' at parameters after variable type on line ";
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

					tok = syntactic->GetNextToken();

					continue;
				}

			}

		}
		return eRETURN_STATE::BAD;
	}
}