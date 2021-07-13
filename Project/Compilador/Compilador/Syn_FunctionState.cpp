#include "pch.h"
#include "Syn_FunctionState.h"

#include "Syn_Parameters.h"
#include "Syn_Statements.h"

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
		Token tok = syntactic->GetNextToken();

		if (tok.GetType() == eTOKEN_TYPE::ID)
		{
			std::string functName = tok.GetLexeme();
			if (syntactic->SymbolExist(functName, "FUNCTION", ""))
			{
				std::string errorMsg = "ID '";
				errorMsg.append(tok.GetLexeme());
				errorMsg.append("' already exist in the current context. On line ");
				errorMsg.append(to_string(tok.GetLine()));
				if (!syntactic->AddError(errorMsg))
				{
					return eRETURN_STATE::FATAL;
				}
			}
			else
			{
				syntactic->AddSymbol(tok.GetLine(), functName, "FUNCTION", 0, "", "int");
			}

			tok = syntactic->GetNextToken();

			if (tok.GetLexeme() == "(")
			{
				SyntaxState* state = new Syn_Parameters();

				eRETURN_STATE r = state->Update(syntactic);

				delete state;

				if (r == eRETURN_STATE::GOOD)
				{
					tok = syntactic->GetNextToken();

					if (tok.GetLexeme() == ":")
					{
						tok = syntactic->GetNextToken();

						if (IsVariableType(tok))
						{
							syntactic->UpdateSymboltype(functName, "FUNCTION", "", tok.GetLexeme());

							tok = syntactic->GetNextToken();

							if (tok.GetLexeme() == "{")
							{
								state = new Syn_Statements();

								r = state->Update(syntactic);

								if (r == eRETURN_STATE::GOOD)
								{
									tok = syntactic->GetNextToken();

									if (tok.GetLexeme() == "}")
									{
										return eRETURN_STATE::GOOD;
									}
									else
									{
										std::string errorMsg = "Expected '}' at the end of statements of function on line ";
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
								else if (r == eRETURN_STATE::FATAL)
								{
									return eRETURN_STATE::FATAL;
								}
							}
							else
							{
								std::string errorMsg = "Expected '{' at the start of statements of function on line ";
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
						else
						{
							std::string errorMsg = "Expected variable type at the end of declaration function on line ";
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
					else
					{
						std::string errorMsg = "Expected ':' after parameters of function declared on line ";
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
				else if (r == eRETURN_STATE::FATAL)
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
				std::string errorMsg = "Expected '(' after ID of function declared on line ";
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
		else
		{
			std::string errorMsg = "Identificator expected at function declaration on line ";
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