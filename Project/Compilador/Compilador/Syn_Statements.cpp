#include "pch.h"
#include "Syn_Statements.h"

#include "Syn_IfState.h"
#include "Syn_FunctionCallState.h"
#include "Syn_AsignState.h"
#include "Syn_PrintState.h"
#include "Syn_ForState.h"
#include "Syn_WhileState.h"
#include "Syn_ReadState.h"
#include "Syn_SwitchState.h"
#include "Syn_ReturnState.h"
#include "Syn_LogicExpresion.h"

namespace Compilador
{
	Syn_Statements::Syn_Statements(bool* hasReturn)
	{
		m_hasReturn = hasReturn;
	}
	Syn_Statements::~Syn_Statements()
	{
	}
	eRETURN_STATE Syn_Statements::Update(AnalizadorSintactico* syntactic)
	{
		bool finished = false;

		Token tok = Token(0, "", eTOKEN_TYPE::NONE);
		SyntaxState* state = nullptr;

		while (!finished)
		{
			state = nullptr;

			// If

			tok = syntactic->GetNextToken();

			if (tok.GetLexeme() == "if")
			{
				state = new Syn_IfState(m_hasReturn);
				if (state->Update(syntactic) == eRETURN_STATE::FATAL)
				{
					delete state;
					return eRETURN_STATE::FATAL;
				}

				delete state;
				state = nullptr;

				continue;
			}
			else
			{
				syntactic->Putback(1);
			}

			// Asign / function call

			tok = syntactic->GetNextToken();

			if (tok.GetType() == eTOKEN_TYPE::ID)
			{
				tok = syntactic->GetNextToken();
				if (tok.GetLexeme() == "(")
				{
					StatementNode* stNode = new StatementNode(eSTATEMENT_TYPE::FUNC_CALL);

					syntactic->Putback(2);
					stNode->m_relatedToken = syntactic->GetNextToken();

					syntactic->StatementTreeAddNode(stNode);

					syntactic->Putback(1);
					std::vector<LogExpNode*> params;
					state = new Syn_FunctionCallState(&params);
					eRETURN_STATE r = state->Update(syntactic);

					for (int p = 0; p < params.size(); p++)
					{
						syntactic->StatementTreeAddLogicTree(params[p]);
					}
					syntactic->StatementTreeReturnToParent();

					delete state;

					if (r == eRETURN_STATE::FATAL)
					{
						return eRETURN_STATE::FATAL;
					}
					else if (r == eRETURN_STATE::BAD)
					{
						syntactic->Putback(1);
						tok = syntactic->GetNextToken();
						if (tok.GetLexeme() == "}")
						{
							syntactic->Putback(1);
							syntactic->StatementTreeReturnToParent();
							return eRETURN_STATE::GOOD;
						}
						else if (tok.GetLexeme() == ";")
						{
							continue;
						}
						syntactic->StatementTreeReturnToParent();
						return eRETURN_STATE::BAD;
					}
					else if (r == eRETURN_STATE::GOOD)
					{
						tok = syntactic->GetNextToken();
						if (tok.GetLexeme() == ";")
						{
							continue;
						}
						else
						{
							std::string errorMsg = "Expected ';' after function call on line ";
							errorMsg.append(to_string(tok.GetLine()));
							if (!syntactic->AddError(errorMsg))
							{
								return eRETURN_STATE::FATAL;
							}

							//Panik Mode
							while (tok.GetLexeme() != ";" && tok.GetLexeme() != "}"
							    && tok.GetType() != eTOKEN_TYPE::END)
							{
								tok = syntactic->GetNextToken();
							}
							if (tok.GetLexeme() == ";")
							{
								continue;
							}
							else if (tok.GetLexeme() == "}")
							{
								syntactic->StatementTreeReturnToParent();
								return eRETURN_STATE::GOOD;
							}
							else if (tok.GetType() == eTOKEN_TYPE::END)
							{
								syntactic->StatementTreeReturnToParent();
								return eRETURN_STATE::BAD;
							}
						}
						return eRETURN_STATE::BAD;
					}
				}
				else if (tok.GetLexeme() == "[")
				{
					LogExpNode* dim;

					StatementNode* stNode = new StatementNode(eSTATEMENT_TYPE::DIM);

					syntactic->Putback(2);
					stNode->m_relatedToken = syntactic->GetNextToken();

					syntactic->StatementTreeAddNode(stNode);

					syntactic->Putback(1);
					state = new Syn_LogicExpresion(&dim);
					eRETURN_STATE r = state->Update(syntactic);
					delete state;
					if (r == eRETURN_STATE::FATAL)
					{
						return eRETURN_STATE::FATAL;
					}
					else if (r == eRETURN_STATE::BAD)
					{
						syntactic->Putback(1);
						tok = syntactic->GetNextToken();
						if (tok.GetLexeme() == "}")
						{
							syntactic->Putback(1);
							return eRETURN_STATE::GOOD;
						}
					}					
				}
				else
				{
					syntactic->Putback(2);
					state = new Syn_AsignState();
					eRETURN_STATE r = state->Update(syntactic);
					delete state;
					if (r == eRETURN_STATE::FATAL)
					{
						return eRETURN_STATE::FATAL;
					}
					else if (r == eRETURN_STATE::BAD)
					{
						syntactic->Putback(1);
						tok = syntactic->GetNextToken();
						if (tok.GetLexeme() == "}")
						{
							syntactic->Putback(1);
							return eRETURN_STATE::GOOD;
						}
					}
				}

				continue;
			}
			else
			{
				syntactic->Putback(1);
			}


			// Print

			tok = syntactic->GetNextToken();

			if (tok.GetLexeme() == "print")
			{
				state = new Syn_PrintState();
				if (state->Update(syntactic) == eRETURN_STATE::FATAL)
				{
					delete state;
					return eRETURN_STATE::FATAL;
				}

				delete state;
				state = nullptr;

				continue;
			}
			else
			{
				syntactic->Putback(1);
			}


			// For

			tok = syntactic->GetNextToken();

			if (tok.GetLexeme() == "for")
			{
				state = new Syn_ForState(m_hasReturn);
				if (state->Update(syntactic) == eRETURN_STATE::FATAL)
				{
					delete state;
					return eRETURN_STATE::FATAL;
				}

				delete state;
				state = nullptr;

				continue;
			}
			else
			{
				syntactic->Putback(1);
			}


			// While

			tok = syntactic->GetNextToken();

			if (tok.GetLexeme() == "while")
			{
				state = new Syn_WhileState(m_hasReturn);
				if (state->Update(syntactic) == eRETURN_STATE::FATAL)
				{
					delete state;
					return eRETURN_STATE::FATAL;
				}

				delete state;
				state = nullptr;

				continue;
			}
			else
			{
				syntactic->Putback(1);
			}


			// Read

			tok = syntactic->GetNextToken();

			if (tok.GetLexeme() == "read")
			{
				state = new Syn_ReadState();
				if (state->Update(syntactic) == eRETURN_STATE::FATAL)
				{
					delete state;
					return eRETURN_STATE::FATAL;
				}

				delete state;
				state = nullptr;

				continue;
			}
			else
			{
				syntactic->Putback(1);
			}


			// Switch

			tok = syntactic->GetNextToken();

			if (tok.GetLexeme() == "switch")
			{
				state = new Syn_SwitchState(m_hasReturn);
				if (state->Update(syntactic) == eRETURN_STATE::FATAL)
				{
					delete state;
					return eRETURN_STATE::FATAL;
				}

				delete state;
				state = nullptr;

				continue;
			}
			else
			{
				syntactic->Putback(1);
			}


			// Return

			tok = syntactic->GetNextToken();

			if (tok.GetLexeme() == "return")
			{
				state = new Syn_ReturnState();
				eRETURN_STATE r = state->Update(syntactic);
				if (r == eRETURN_STATE::FATAL)
				{
					delete state;
					return eRETURN_STATE::FATAL;
				}
				else if (r == eRETURN_STATE::GOOD)
				{
					*m_hasReturn = true;
				}

				delete state;
				state = nullptr;

				continue;
			}
			else
			{
				syntactic->Putback(1);
			}

			// End

			tok = syntactic->GetNextToken();

			if (tok.GetLexeme() == "}")
			{
				finished = true;

				syntactic->Putback(1);

				continue;
			}
			else
			{
				std::string errorMsg = "Invalid Token: ";
				errorMsg.append(tok.GetLexeme());
				errorMsg.append(" on line ");
				errorMsg.append(to_string(tok.GetLine()));
				if (!syntactic->AddError(errorMsg))
				{
					return eRETURN_STATE::FATAL;
				}
			}
		}

		return eRETURN_STATE::GOOD;
	}
}