#include "pch.h"
#include "AnalizadorSemantico.h"

namespace Compilador
{
	AnalizadorSemantico::AnalizadorSemantico(AnalizadorSintactico* syntactic, 
	                                         ErrorManager^ errorMan)
	{
		m_syntactic = syntactic;
		m_errorManager = errorMan;
	}
	AnalizadorSemantico::~AnalizadorSemantico()
	{
	}
	bool AnalizadorSemantico::Parce(std::map<std::string, StatementNode*> funcTrees)
	{
		m_intermidiateCode = "";
		m_functionStatementsTrees = funcTrees;

		for (std::map<std::string, StatementNode*>::iterator it= m_functionStatementsTrees.begin();
			it != m_functionStatementsTrees.end(); it++)
		{
			if (!SearchStatementTrees(it->second, it->first))
			{
				return false;
			}
		}
		//TODO: Agregar else, switch, case. Manejar errores fatales en esta sección.
		return true;
	}


	bool AnalizadorSemantico::AddError(std::string errorString)
	{
		std::string errorMsg = "<SEMANTIC> ";
		errorMsg.append(errorString);
		return m_errorManager->AddError(gcnew String(errorMsg.c_str()), eERROR_TYPE::SEMANTIC);
	}


	std::string AnalizadorSemantico::LogicTreeChecker(LogExpNode* node, std::string functionName)
	{
		if (node->m_value.GetType() == eTOKEN_TYPE::CHAR_CONST)
		{
			return "char";
		}
		else if (node->m_value.GetType() == eTOKEN_TYPE::FLOAT_CONST)
		{
			return "float";
		}
		else if (node->m_value.GetType() == eTOKEN_TYPE::ID)
		{
			bool isFunction = false;
			std::string type = m_syntactic->GetSymbolType(node->m_value.GetLexeme(), 
														                 functionName, isFunction);
			if (isFunction && node->m_extraInfo == eNODE_INFO::FUNC)
			{
				std::map<int, std::string> parameters = 
												  m_syntactic->GetFuncParameterTypes(node->m_value.GetLexeme());

				if (node->m_subTrees.size() > parameters.size())
				{
					std::string errorMsg = "Too many parameters on call function ";
					errorMsg.append(node->m_value.GetLexeme());
					errorMsg.append(" on line ");
					errorMsg.append(to_string(node->m_value.GetLine()));
					if (!AddError(errorMsg))
					{
						return "fatal";
					}
				}
				else if (node->m_subTrees.size() < parameters.size())
				{
					std::string errorMsg = "Not enough parameters on call function ";
					errorMsg.append(node->m_value.GetLexeme());
					errorMsg.append(" on line ");
					errorMsg.append(to_string(node->m_value.GetLine()));
					if (!AddError(errorMsg))
					{
						return "fatal";
					}
				}
				
				for (int i = 0; i < node->m_subTrees.size(); i++)
				{
					std::string resultType = LogicTreeChecker(node->m_subTrees[i], functionName);
					if (resultType == "fatal")
					{
						return "fatal";
					}

					if (i < parameters.size() && parameters[i] != resultType)
					{
						std::string errorMsg = "Invalid parameter type <";
						errorMsg.append(resultType != "" ? resultType : "error_type");
						errorMsg.append("> on call function ");
						errorMsg.append(node->m_value.GetLexeme());
						errorMsg.append(" on line ");
						errorMsg.append(to_string(node->m_value.GetLine()));
						if (!AddError(errorMsg))
						{
							return "fatal";
						}
					}
					else if (i < parameters.size())
					{
						CreateIntermidiateCode(node->m_subTrees[i], functionName);
					}
				}

				return type;
			}
			else if (!isFunction && node->m_extraInfo == eNODE_INFO::DIM)
			{
				std::string resultType = LogicTreeChecker(node->m_subTrees[0], functionName);
				if (resultType != "int")
				{
					std::string errorMsg = "Invalid type  <";
					errorMsg.append(resultType != "" ? resultType : "error_type");
					errorMsg.append(">  on dimension on line ");
					errorMsg.append(to_string(node->m_value.GetLine()));
					if (!AddError(errorMsg))
					{
						return "fatal";
					}

					return "";
				}
				else
				{
					CreateIntermidiateCode(node->m_subTrees[0], functionName);
					return type;
				}
			}
			else if (!isFunction)
			{
				return type;
			}
			else
			{
				std::string errorMsg = "ID  <";
				errorMsg.append(node->m_value.GetLexeme());
				errorMsg.append(" is not a function. On line ");
				errorMsg.append(to_string(node->m_value.GetLine()));
				if (!AddError(errorMsg))
				{
					return "fatal";
				}

				return type;
			}
		}
		else if (node->m_value.GetType() == eTOKEN_TYPE::INT_CONST)
		{
			return "int";
		}
		else if (node->m_value.GetType() == eTOKEN_TYPE::STRING_CONST)
		{
			return "string";
		}
		else if (node->m_value.GetType() == eTOKEN_TYPE::LOGIC_CONST)
		{
			return "bool";
		}
		else
		{
			std::string left = "";
			if (node->m_left != nullptr)
			{
				left = LogicTreeChecker(node->m_left, functionName);
				if (left == "fatal")
				{
					return "fatal";
				}
			}

			std::string right = "";
			if (node->m_right != nullptr)
			{
				right = LogicTreeChecker(node->m_right, functionName);
				if (right == "fatal")
				{
					return "fatal";
				}
			}

			if (node->m_value.GetLexeme() == "||")
			{
				if (left == "bool" && right == "bool")
				{
					return "bool";
				}
				else
				{
					std::string errorMsg = "There is no operator overlad for types: <";
					errorMsg.append(left != "" ? left : "error_type");
					errorMsg.append("> || <");
					errorMsg.append(right != "" ? right : "error_type");
					errorMsg.append(">. Types are: <bool> || <bool>. On line ");
					errorMsg.append(to_string(node->m_value.GetLine()));
					if (!AddError(errorMsg))
					{
						return "fatal";
					}

					return "";
				}
			}
			else if (node->m_value.GetLexeme() == "&&")
			{
				if (left == "bool" && right == "bool")
				{
					return "bool";
				}
				else
				{
					std::string errorMsg = "There is no operator overlad for types: <";
					errorMsg.append(left != "" ? left : "error_type");
					errorMsg.append("> && <");
					errorMsg.append(right != "" ? right : "error_type");
					errorMsg.append(">. Types are: <bool> && <bool>. On line ");
					errorMsg.append(to_string(node->m_value.GetLine()));
					if (!AddError(errorMsg))
					{
						return "fatal";
					}

					return "";
				}
			}
			else if (node->m_value.GetLexeme() == "==")
			{
				if (left == right)
				{
					return "bool";
				}
				else
				{
					std::string errorMsg = "There is no operator overlad for types: <";
					errorMsg.append(left != "" ? left : "error_type");
					errorMsg.append("> == <");
					errorMsg.append(right != "" ? right : "error_type");
					errorMsg.append(">. On line ");
					errorMsg.append(to_string(node->m_value.GetLine()));
					if (!AddError(errorMsg))
					{
						return "fatal";
					}

					return "";
				}
			}
			else if (node->m_value.GetLexeme() == "!=")
			{
				if (left == right)
				{
					return "bool";
				}
				else
				{
					std::string errorMsg = "There is no operator overlad for types: <";
					errorMsg.append(left != "" ? left : "error_type");
					errorMsg.append("> != <");
					errorMsg.append(right != "" ? right : "error_type");
					errorMsg.append(">. On line ");
					errorMsg.append(to_string(node->m_value.GetLine()));
					if (!AddError(errorMsg))
					{
						return "fatal";
					}

					return "";
				}
			}
			else if (node->m_value.GetLexeme() == "<")
			{
				if ((left == "int" || left == "float") && (right == "int" || right == "float"))
				{
					return "bool";
				}
				else
				{
					std::string errorMsg = "There is no operator overlad for types: <";
					errorMsg.append(left != "" ? left : "error_type");
					errorMsg.append("> < <");
					errorMsg.append(right != "" ? right : "error_type");
					errorMsg.append(">. Types are: <int|float> < <int|float>. On line ");
					errorMsg.append(to_string(node->m_value.GetLine()));
					if (!AddError(errorMsg))
					{
						return "fatal";
					}

					return "";
				}
			}
			else if (node->m_value.GetLexeme() == "<=")
			{
				if ((left == "int" || left == "float") && (right == "int" || right == "float"))
				{
					return "bool";
				}
				else
				{
					std::string errorMsg = "There is no operator overlad for types: <";
					errorMsg.append(left != "" ? left : "error_type");
					errorMsg.append("> <= <");
					errorMsg.append(right != "" ? right : "error_type");
					errorMsg.append(">. Types are: <int|float> <= <int|float>. On line ");
					errorMsg.append(to_string(node->m_value.GetLine()));
					if (!AddError(errorMsg))
					{
						return "fatal";
					}

					return "";
				}
			}
			else if (node->m_value.GetLexeme() == ">")
			{
				if ((left == "int" || left == "float") && (right == "int" || right == "float"))
				{
					return "bool";
				}
				else
				{
					std::string errorMsg = "There is no operator overlad for types: <";
					errorMsg.append(left != "" ? left : "error_type");
					errorMsg.append("> > <");
					errorMsg.append(right != "" ? right : "error_type");
					errorMsg.append(">. Types are: <int|float> > <int|float>. On line ");
					errorMsg.append(to_string(node->m_value.GetLine()));
					if (!AddError(errorMsg))
					{
						return "fatal";
					}

					return "";
				}
			}
			else if (node->m_value.GetLexeme() == ">=")
			{
				if ((left == "int" || left == "float") && (right == "int" || right == "float"))
				{
					return "bool";
				}
				else
				{
					std::string errorMsg = "There is no operator overlad for types: <";
					errorMsg.append(left != "" ? left : "error_type");
					errorMsg.append("> >= <");
					errorMsg.append(right != "" ? right : "error_type");
					errorMsg.append(">. Types are: <int|float> >= <int|float>. On line ");
					errorMsg.append(to_string(node->m_value.GetLine()));
					if (!AddError(errorMsg))
					{
						return "fatal";
					}

					return "";
				}
			}
			else if (node->m_value.GetLexeme() == "+")
			{
				if (left == "int" && right == "int")
				{
					return "int";
				}
				else if ((left == "int" || left == "float") 
				     && (right == "int" || right == "float"))
				{
					return "float";
				}
				else if (left == "string" || right == "string")
				{
					return "string";
				}
				else
				{
					std::string errorMsg = "There is no operator overlad for types: <";
					errorMsg.append(left != "" ? left : "error_type");
					errorMsg.append("> + <");
					errorMsg.append(right != "" ? right : "error_type");
					errorMsg.append(">. On line ");
					errorMsg.append(to_string(node->m_value.GetLine()));
					if (!AddError(errorMsg))
					{
						return "fatal";
					}

					return "";
				}
			}
			else if (node->m_value.GetLexeme() == "-" 
			      && node->m_value.GetType() == eTOKEN_TYPE::ARITHMETIC_OPERATOR)
			{
				if (left == "int" && right == "int")
				{
					return "int";
				}
				else if ((left == "int" || left == "float") 
				     && (right == "int" || right == "float"))
				{
					return "float";
				}
				else
				{
					std::string errorMsg = "There is no operator overlad for types: <";
					errorMsg.append(left != "" ? left : "error_type");
					errorMsg.append("> - <");
					errorMsg.append(right != "" ? right : "error_type");
					errorMsg.append(">. Types are: <int|float> - <int|float>. On line ");
					errorMsg.append(to_string(node->m_value.GetLine()));
					if (!AddError(errorMsg))
					{
						return "fatal";
					}

					return "";
				}
			}
			else if (node->m_value.GetLexeme() == "*")
			{
				if (left == "int" && right == "int")
				{
					return "int";
				}
				else if ((left == "int" || left == "float") 
				     && (right == "int" || right == "float"))
				{
					return "float";
				}
				else
				{
					std::string errorMsg = "There is no operator overlad for types: <";
					errorMsg.append(left != "" ? left : "error_type");
					errorMsg.append("> * <");
					errorMsg.append(right != "" ? right : "error_type");
					errorMsg.append(">. Types are: <int|float> * <int|float>. On line ");
					errorMsg.append(to_string(node->m_value.GetLine()));
					if (!AddError(errorMsg))
					{
						return "fatal";
					}

					return "";
				}
			}
			else if (node->m_value.GetLexeme() == "/")
			{
				if (left == "int" && right == "int")
				{
					return "int";
				}
				else if ((left == "int" || left == "float") 
				     && (right == "int" || right == "float"))
				{
					return "float";
				}
				else
				{
					std::string errorMsg = "There is no operator overlad for types: <";
					errorMsg.append(left != "" ? left : "error_type");
					errorMsg.append("> / <");
					errorMsg.append(right != "" ? right : "error_type");
					errorMsg.append(">. Types are: <int|float> / <int|float>. On line ");
					errorMsg.append(to_string(node->m_value.GetLine()));
					if (!AddError(errorMsg))
					{
						return "fatal";
					}

					return "";
				}
			}
			else if (node->m_value.GetLexeme() == "%")
			{
				if (left == "int" && right == "int")
				{
					return "int";
				}
				else
				{
					std::string errorMsg = "There is no operator overlad for types: <";
					errorMsg.append(left != "" ? left : "error_type");
					errorMsg.append("> % <");
					errorMsg.append(right != "" ? right : "error_type");
					errorMsg.append(">. Types are: <int> % <int>. On line ");
					errorMsg.append(to_string(node->m_value.GetLine()));
					if (!AddError(errorMsg))
					{
						return "fatal";
					}

					return "";
				}
			}
			else if (node->m_value.GetLexeme() == "^")
			{
				if (left == "int" && right == "int")
				{
					return "int";
				}
				else if ((left == "int" || left == "float") 
				     && (right == "int" || right == "float"))
				{
					return "float";
				}
				else
				{
					std::string errorMsg = "There is no operator overlad for types: <";
					errorMsg.append(left != "" ? left : "error_type");
					errorMsg.append("> ^ <");
					errorMsg.append(right != "" ? right : "error_type");
					errorMsg.append(">. Types are: <int|float> ^ <int|float>. On line ");
					errorMsg.append(to_string(node->m_value.GetLine()));
					if (!AddError(errorMsg))
					{
						return "fatal";
					}

					return "";
				}
			}
			else if (node->m_value.GetLexeme() == "!")
			{
				if (right == "bool")
				{
					return "bool";
				}
				else
				{
					std::string errorMsg = "There is no operator overlad for types:  !<";
					errorMsg.append(right != "" ? right : "error_type");
					errorMsg.append(">. Types are: !<bool>. On line ");
					errorMsg.append(to_string(node->m_value.GetLine()));
					if (!AddError(errorMsg))
					{
						return "fatal";
					}

					return "";
				}
			}
			else if (node->m_value.GetLexeme() == "-" 
				  && node->m_value.GetType() == eTOKEN_TYPE::UNARY)
			{
				if (right == "int" || right == "float")
				{
					return right;
				}
				else
				{
					std::string errorMsg = "There is no operator overlad for types: -<";
					errorMsg.append(right != "" ? right : "error_type");
					errorMsg.append(">. Types are: -<int|float>. On line ");
					errorMsg.append(to_string(node->m_value.GetLine()));
					if (!AddError(errorMsg))
					{
						return "fatal";
					}

					return "";
				}
			}
		}
		return "";
	}
	std::string AnalizadorSemantico::CreateIntermidiateCode(LogExpNode* node, 
	                                                        std::string functionName)
	{
		std::string r;
		if (node->m_right != nullptr)
		{
			r = CreateIntermidiateCode(node->m_right, functionName);
		}
		else
		{
			if (node->m_value.GetType() == eTOKEN_TYPE::CHAR_CONST
			 || node->m_value.GetType() == eTOKEN_TYPE::STRING_CONST
			 || node->m_value.GetType() == eTOKEN_TYPE::FLOAT_CONST
		     || node->m_value.GetType() == eTOKEN_TYPE::INT_CONST 
			 || node->m_value.GetType() == eTOKEN_TYPE::LOGIC_CONST)
			{
				return "LIT " + node->m_value.GetLexeme() + ", 0\n";
			}
			else if (node->m_value.GetType() == eTOKEN_TYPE::ID)
			{
				std::string result;
				bool isFunction = false;
				m_syntactic->GetSymbolType(node->m_value.GetLexeme(), functionName, isFunction);
				if (isFunction && node->m_extraInfo == eNODE_INFO::FUNC)
				{
					std::map<int, std::string> parameters = 
												  m_syntactic->GetFuncParameterTypes(node->m_value.GetLexeme());

					for (int i = node->m_subTrees.size() - 1; i >= 0; i--)
					{
						if (i < parameters.size())
						{
							result += CreateIntermidiateCode(node->m_subTrees[i], functionName);
							result += "STO 0, @VAR" + to_string(i) + "\n";
						}
					}
					for (int i = node->m_subTrees.size() - 1; i >= 0; i--)
					{
						result += "LOD @VAR" + to_string(i) + ", 0\n";
					}
					result += "CAL 0, " + node->m_value.GetLexeme() + "\n";
				}
				else if (!isFunction && node->m_extraInfo == eNODE_INFO::DIM)
				{
					result = CreateIntermidiateCode(node->m_subTrees[0], functionName);
					result += "STO 0, @VAR0\n";
					result += "LODX " + node->m_value.GetLexeme() + ", @VAR0\n";
				}
				else
				{
					result = "LOD " + node->m_value.GetLexeme() + ", 0\n";
				}

				return result;
			}
			else
			{
				return "LIT 0, 0\n";
			}
		}



		if (node->m_value.GetLexeme() == "||")
		{
			r += CreateIntermidiateCode(node->m_left, functionName);
			return r + "OPR 0, 15\n";
		}
		else if (node->m_value.GetLexeme() == "&&")
		{
			r += CreateIntermidiateCode(node->m_left, functionName);
			return r + "OPR 0, 14\n";
		}
		else if (node->m_value.GetLexeme() == "==")
		{
			r += CreateIntermidiateCode(node->m_left, functionName);
			return r + "OPR 0, 12\n";
		}
		else if (node->m_value.GetLexeme() == "!=")
		{
			r += CreateIntermidiateCode(node->m_left, functionName);
			return r + "OPR 0, 13\n";
		}
		else if (node->m_value.GetLexeme() == "<")
		{
			r += CreateIntermidiateCode(node->m_left, functionName);
			return r + "OPR 0, 10\n";
		}
		else if (node->m_value.GetLexeme() == "<=")
		{
			r += CreateIntermidiateCode(node->m_left, functionName);
			return r + "OPR 0, 11\n";
		}
		else if (node->m_value.GetLexeme() == ">")
		{
			r += CreateIntermidiateCode(node->m_left, functionName);
			return r + "OPR 0, 8\n";
		}
		else if (node->m_value.GetLexeme() == ">=")
		{
			r += CreateIntermidiateCode(node->m_left, functionName);
			return r + "OPR 0, 9\n";
		}
		else if (node->m_value.GetLexeme() == "+")
		{
			r += CreateIntermidiateCode(node->m_left, functionName);
			return r + "OPR 0, 2\n";
		}
		else if (node->m_value.GetLexeme() == "-" 
		      && node->m_value.GetType() == eTOKEN_TYPE::ARITHMETIC_OPERATOR)
		{
			r += CreateIntermidiateCode(node->m_left, functionName);
			return r + "OPR 0, 3\n";
		}
		else if (node->m_value.GetLexeme() == "*")
		{
			r += CreateIntermidiateCode(node->m_left, functionName);
			return r + "OPR 0, 4\n";
		}
		else if (node->m_value.GetLexeme() == "/")
		{
			r += CreateIntermidiateCode(node->m_left, functionName);
			return r + "OPR 0, 5\n";
		}
		else if (node->m_value.GetLexeme() == "%")
		{
			r += CreateIntermidiateCode(node->m_left, functionName);
			return r + "OPR 0, 6\n";
		}
		else if (node->m_value.GetLexeme() == "^")
		{
			r += CreateIntermidiateCode(node->m_left, functionName);
			return r + "OPR 0, 7\n";
		}
		else if (node->m_value.GetLexeme() == "!")
		{
			return r + "OPR 0, 16\n";
		}
		else if (node->m_value.GetLexeme() == "-" && node->m_value.GetType() == eTOKEN_TYPE::UNARY)
		{
			r += "LIT 0, 0\n";
			return r + "OPR 0, 3\n";
		}

		return "LIT 0, 0\n";
	}
	bool AnalizadorSemantico::SearchStatementTrees(StatementNode* node, std::string funcName)
	{
		if (node->m_type == eSTATEMENT_TYPE::ASIGN)
		{
			if (node->m_extraInfo == eEXTRA_INFO::ASING_DIM && node->m_logExpresions.size() > 0)
			{
				std::string dimType = LogicTreeChecker(node->m_logExpresions[0], funcName);
				if (dimType == "fatal")
				{
					return false;
				}

				if (dimType != "int")
				{
					std::string errorMsg = "Invalid type <";
					errorMsg.append(dimType != "" ? dimType : "error_type");
					errorMsg.append("> on dimension on line ");
					errorMsg.append(to_string(node->m_relatedToken.GetLine()));
					if (!AddError(errorMsg))
					{
						return false;
					}
				}
				if (dimType != "")
				{
					m_intermidiateCode += CreateIntermidiateCode(node->m_logExpresions[0], funcName);
					m_intermidiateCode += "\n";
				}
			}

			std::string type = "int";
			if (node->m_logExpresions.size() > (node->m_extraInfo == eEXTRA_INFO::ASING_DIM ? 1 : 0))
			{
				type = LogicTreeChecker(node->m_logExpresions[(node->m_extraInfo == eEXTRA_INFO::ASING_DIM ? 1 : 0)], funcName);
				if (type == "fatal")
				{
					return false;
				}
			}

			bool isFunction = false;
			if (type != 
			    m_syntactic->GetSymbolType(node->m_relatedToken.GetLexeme(), funcName, isFunction))
			{
				std::string errorMsg = "Invalid type <";
				errorMsg.append(type != "" ? type : "error_type");
				errorMsg.append("> on asignment on line ");
				errorMsg.append(to_string(node->m_relatedToken.GetLine()));
				if (!AddError(errorMsg))
				{
					return false;
				}
			}
			if (type != "")
			{
				m_intermidiateCode += CreateIntermidiateCode(node->m_logExpresions[(node->m_extraInfo == eEXTRA_INFO::ASING_DIM ? 1 : 0)], funcName);
				m_intermidiateCode += "\n";
			}
		}
		else if (node->m_type == eSTATEMENT_TYPE::CASE)
		{
			std::string type = "int";
			if (node->m_logExpresions.size() > 0)
			{
				type = LogicTreeChecker(node->m_logExpresions[0], funcName);
				if (type == "fatal")
				{
					return false;
				}
			}

			bool isFunction = false;
			if (type != 
			    m_syntactic->GetSymbolType(node->m_relatedToken.GetLexeme(), funcName, isFunction))
			{
				std::string errorMsg = "Invalid type <";
				errorMsg.append(type != "" ? type : "error_type");
				errorMsg.append("> on case on line ");
				errorMsg.append(to_string(node->m_relatedToken.GetLine()));
				if (!AddError(errorMsg))
				{
					return false;
				}
			}
			if (type != "")
			{
				m_intermidiateCode += CreateIntermidiateCode(node->m_logExpresions[0], funcName);
				m_intermidiateCode += "\n";
			}

			for (int i = 0; i < node->m_subStatements.size(); i++)
			{
				if (!SearchStatementTrees(node->m_subStatements[i], funcName))
				{
					return false;
				}
			}
		}
		else if (node->m_type == eSTATEMENT_TYPE::DEFAULT)
		{
			for (int i = 0; i < node->m_subStatements.size(); i++)
			{
				if (!SearchStatementTrees(node->m_subStatements[i], funcName))
				{
					return false;
				}
			}
		}
		else if (node->m_type == eSTATEMENT_TYPE::FOR)
		{
			std::string type = "bool";
			if (node->m_logExpresions.size() > 0)
			{
				type = LogicTreeChecker(node->m_logExpresions[0], funcName);
				if (type == "fatal")
				{
					return false;
				}
			}

			bool isFunction = false;
			if (type != "bool")
			{
				std::string errorMsg = "Invalid type <";
				errorMsg.append(type != "" ? type : "error_type");
				errorMsg.append("> on for on line ");
				errorMsg.append(to_string(node->m_relatedToken.GetLine()));
				if (!AddError(errorMsg))
				{
					return false;
				}
			}
			if (type != "")
			{
				m_intermidiateCode += CreateIntermidiateCode(node->m_logExpresions[0], funcName);
				m_intermidiateCode += "\n";
			}

			for (int i = 0; i < node->m_subStatements.size(); i++)
			{
				if (!SearchStatementTrees(node->m_subStatements[i], funcName))
				{
					return false;
				}
			}
		}
		else if (node->m_type == eSTATEMENT_TYPE::FUNC_CALL)
		{
			std::map<int, std::string> parameters = 
			                  m_syntactic->GetFuncParameterTypes(node->m_relatedToken.GetLexeme());

			if (node->m_logExpresions.size() > parameters.size())
			{
				std::string errorMsg = "Too many parameters on call function ";
				errorMsg.append(node->m_relatedToken.GetLexeme());
				errorMsg.append(" on line ");
				errorMsg.append(to_string(node->m_relatedToken.GetLine()));
				if (!AddError(errorMsg))
				{
					return false;
				}
			}
			else if (node->m_logExpresions.size() < parameters.size())
			{
				std::string errorMsg = "Not enough parameters on call function ";
				errorMsg.append(node->m_relatedToken.GetLexeme());
				errorMsg.append(" on line ");
				errorMsg.append(to_string(node->m_relatedToken.GetLine()));
				if (!AddError(errorMsg))
				{
					return false;
				}
			}

			for (int i = 0; i < node->m_logExpresions.size(); i++)
			{
				if (i < parameters.size())
				{
					std::string resultType = LogicTreeChecker(node->m_logExpresions[i], funcName);
					if (resultType == "fatal")
					{
						return false;
					}

					if (parameters[i] != resultType)
					{
						std::string errorMsg = "Invalid parameter type <";
						errorMsg.append(resultType != "" ? resultType : "error_type");
						errorMsg.append("> on call function ");
						errorMsg.append(node->m_relatedToken.GetLexeme());
						errorMsg.append(" on line ");
						errorMsg.append(to_string(node->m_relatedToken.GetLine()));
						if (!AddError(errorMsg))
						{
							return false;
						}
					}
					if (resultType != "")
					{
						m_intermidiateCode += CreateIntermidiateCode(node->m_logExpresions[i], funcName);
						m_intermidiateCode += "\n";
					}
				}
			}
		}
		else if (node->m_type == eSTATEMENT_TYPE::IF)
		{
			std::string type = "bool";
			if (node->m_logExpresions.size() > 0)
			{
				type = LogicTreeChecker(node->m_logExpresions[0], funcName);
				if (type == "fatal")
				{
					return false;
				}
			}

			bool isFunction = false;
			if (type != "bool")
			{
				std::string errorMsg = "Invalid type <";
				errorMsg.append(type != "" ? type : "error_type");
				errorMsg.append("> on if on line ");
				errorMsg.append(to_string(node->m_relatedToken.GetLine()));
				if (!AddError(errorMsg))
				{
					return false;
				}
			}
			if (type != "")
			{
				m_intermidiateCode += CreateIntermidiateCode(node->m_logExpresions[0], funcName);
				m_intermidiateCode += "\n";
			}

			for (int i = 0; i < node->m_subStatements.size(); i++)
			{
				if (!SearchStatementTrees(node->m_subStatements[i], funcName))
				{
					return false;
				}
			}
		}
		else if (node->m_type == eSTATEMENT_TYPE::PRINT)
		{
			for (int i = 0; i < node->m_logExpresions.size(); i++)
			{
				std::string type = LogicTreeChecker(node->m_logExpresions[i], funcName);
				if (type == "fatal")
				{
					return false;
				}

				if (type != "")
				{
					m_intermidiateCode += CreateIntermidiateCode(node->m_logExpresions[i], funcName);
					m_intermidiateCode += "\n";
				}
			}
		}
		else if (node->m_type == eSTATEMENT_TYPE::RETURN)
		{
			std::string type = "void";
			if (node->m_logExpresions.size() > 0)
			{
				type = LogicTreeChecker(node->m_logExpresions[0], funcName);
				if (type == "fatal")
				{
					return false;
				}
			}

			bool isFunction = false;
			if (type != m_syntactic->GetSymbolType(funcName, "", isFunction))
			{
				std::string errorMsg = "Invalid return type <";
				errorMsg.append(type != "" ? type : "error_type");
				errorMsg.append("> on line ");
				errorMsg.append(to_string(node->m_relatedToken.GetLine()));
				if (!AddError(errorMsg))
				{
					return false;
				}
			}
			if (type != "")
			{
				if (node->m_logExpresions.size() > 0)
				{
					m_intermidiateCode += CreateIntermidiateCode(node->m_logExpresions[0], funcName);
					m_intermidiateCode += "\n";
				}
			}
		}
		else if (node->m_type == eSTATEMENT_TYPE::SWITCH)
		{
			for (int i = 0; i < node->m_subStatements.size(); i++)
			{
				if (!SearchStatementTrees(node->m_subStatements[i], funcName))
				{
					return false;
				}
			}
		}
		else if (node->m_type == eSTATEMENT_TYPE::WHILE)
		{
			std::string type = "bool";
			if (node->m_logExpresions.size() > 0)
			{
				type = LogicTreeChecker(node->m_logExpresions[0], funcName);
				if (type == "fatal")
				{
					return false;
				}
			}

			bool isFunction = false;
			if (type != "bool")
			{
				std::string errorMsg = "Invalid type <";
				errorMsg.append(type != "" ? type : "error_type");
				errorMsg.append("> on while on line ");
				errorMsg.append(to_string(node->m_relatedToken.GetLine()));
				if (!AddError(errorMsg))
				{
					return false;
				}
			}
			if (type != "")
			{
				m_intermidiateCode += CreateIntermidiateCode(node->m_logExpresions[0], funcName);
				m_intermidiateCode += "\n";
			}

			for (int i = 0; i < node->m_subStatements.size(); i++)
			{
				if (!SearchStatementTrees(node->m_subStatements[i], funcName))
				{
					return false;
				}
			}
		}
		else if (node->m_type == eSTATEMENT_TYPE::NONE)
		{
			for (int i = 0; i < node->m_subStatements.size(); i++)
			{
				if (!SearchStatementTrees(node->m_subStatements[i], funcName))
				{
					return false;
				}
			}
		}
		return true;
	}
	std::string AnalizadorSemantico::GetIntermidiateCode()
	{
		return m_intermidiateCode;
	}
}