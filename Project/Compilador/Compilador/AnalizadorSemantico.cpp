#include "pch.h"
#include "AnalizadorSemantico.h"

namespace Compilador
{
	AnalizadorSemantico::AnalizadorSemantico(AnalizadorSintactico* syntactic, ErrorManager^ errorMan)
	{
		m_syntactic = syntactic;
		m_errorManager = errorMan;
	}
	AnalizadorSemantico::~AnalizadorSemantico()
	{
	}
	void AnalizadorSemantico::Parce(std::vector<Token> m_tokens)
	{
		m_tokens = m_tokens;

		for (int i = 0; i < m_syntactic->m_logicTrees.size(); i++)
		{
			LogicTreeChecker(m_syntactic->m_logicTrees[i][0], "");
		}
	}


	bool AnalizadorSemantico::AddError(std::string errorString)
	{
		std::string errorMsg = "<SEMANTIC> ";
		errorMsg.append(errorString);
		return m_errorManager->AddError(gcnew String(errorMsg.c_str()), eERROR_TYPE::SYNTACTIC);
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
			std::string type = m_syntactic->GetSymbolType(node->m_value.GetLexeme(), functionName, isFunction);
			if (isFunction)
			{
				std::map<int, std::string> parameters = m_syntactic->GetFuncParameterTypes(functionName);

				if (node->m_subTrees.size() > parameters.size())
				{
					std::string errorMsg = "Too many parameters on call function ";
					errorMsg.append(node->m_value.GetLexeme());
					errorMsg.append(" on line ");
					errorMsg.append(to_string(node->m_value.GetLine()));
					AddError(errorMsg);
				}
				else if (node->m_subTrees.size() > parameters.size())
				{
					std::string errorMsg = "Not enough parameters on call function ";
					errorMsg.append(node->m_value.GetLexeme());
					errorMsg.append(" on line ");
					errorMsg.append(to_string(node->m_value.GetLine()));
					AddError(errorMsg);
				}
				
				for (int i = 0; i < node->m_subTrees.size(); i++)
				{
					if (i < parameters.size())
					{
						std::string resultType = LogicTreeChecker(node->m_subTrees[i], functionName);
						if (parameters[i] != resultType)
						{
							std::string errorMsg = "Invalid parameter type <";
							errorMsg.append(resultType != "" ? resultType : "error_type");
							errorMsg.append("> on call function ");
							errorMsg.append(node->m_value.GetLexeme());
							errorMsg.append(" on line ");
							errorMsg.append(to_string(node->m_value.GetLine()));
							AddError(errorMsg);
						}
					}
				}
			}
			return type;
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
			}

			std::string right = "";
			if (node->m_right != nullptr)
			{
				right = LogicTreeChecker(node->m_right, functionName);
			}

			if (node->m_value.GetLexeme() == "||")
			{
				if (left == "bool" && right == "bool")
				{
					return "bool";
				}
				else
				{
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
					errorMsg.append("> || <");
					errorMsg.append(right != "" ? right : "error_type");
					errorMsg.append(">. Types are: <bool> || <bool>. On line ");
					errorMsg.append(to_string(node->m_value.GetLine()));
					AddError(errorMsg);

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
					AddError(errorMsg);

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
					AddError(errorMsg);

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
					AddError(errorMsg);

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
					AddError(errorMsg);

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
					AddError(errorMsg);

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
					AddError(errorMsg);

					return "";
				}
			}
			else if (node->m_value.GetLexeme() == "+")
			{
				if (left == "int" && right == "int")
				{
					return "int";
				}
				else if ((left == "int" || left == "float") && (right == "int" || right == "float"))
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
					AddError(errorMsg);

					return "";
				}
			}
			else if (node->m_value.GetLexeme() == "-" && node->m_value.GetType() == eTOKEN_TYPE::ARITHMETIC_OPERATOR)
			{
				if (left == "int" && right == "int")
				{
					return "int";
				}
				else if ((left == "int" || left == "float") && (right == "int" || right == "float"))
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
					AddError(errorMsg);

					return "";
				}
			}
			else if (node->m_value.GetLexeme() == "*")
			{
				if (left == "int" && right == "int")
				{
					return "int";
				}
				else if ((left == "int" || left == "float") && (right == "int" || right == "float"))
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
					AddError(errorMsg);

					return "";
				}
			}
			else if (node->m_value.GetLexeme() == "/")
			{
				if (left == "int" && right == "int")
				{
					return "int";
				}
				else if ((left == "int" || left == "float") && (right == "int" || right == "float"))
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
					AddError(errorMsg);

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
					AddError(errorMsg);

					return "";
				}
			}
			else if (node->m_value.GetLexeme() == "^")
			{
				if (left == "int" && right == "int")
				{
					return "int";
				}
				else if ((left == "int" || left == "float") && (right == "int" || right == "float"))
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
					AddError(errorMsg);

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
					AddError(errorMsg);

					return "";
				}
			}
			else if (node->m_value.GetLexeme() == "-" && node->m_value.GetType() == eTOKEN_TYPE::UNARY)
			{
				if (right == "int" || right == "float")
				{
					return right;
				}
				else
				{
					std::string errorMsg = "There is no operator overlad for types:  -<";
					errorMsg.append(right != "" ? right : "error_type");
					errorMsg.append(">. Types are: -<int|float>. On line ");
					errorMsg.append(to_string(node->m_value.GetLine()));
					AddError(errorMsg);

					return "";
				}
			}
		}
		return "";
	}
	std::string AnalizadorSemantico::CreateIntermidiateCode(LogExpNode* node, int& varIndex, std::string functionName)
	{
		std::string r;
		if (node->m_right != nullptr)
		{
			r = CreateIntermidiateCode(node->m_right, varIndex, functionName);
		}
		else
		{
			if (node->m_value.GetType() == eTOKEN_TYPE::CHAR_CONST)
			{
				return "LIT '" + node->m_value.GetLexeme() + "', 0\n";
			}
			else if (node->m_value.GetType() == eTOKEN_TYPE::STRING_CONST)
			{
				return "LIT \"" + node->m_value.GetLexeme() + "\", 0\n";
			}
			else if (node->m_value.GetType() == eTOKEN_TYPE::FLOAT_CONST || node->m_value.GetType() == eTOKEN_TYPE::INT_CONST 
			       ||node->m_value.GetType() == eTOKEN_TYPE::LOGIC_CONST)
			{
				return "LIT " + node->m_value.GetLexeme() + " , 0\n";
			}
			else if (node->m_value.GetType() == eTOKEN_TYPE::ID)
			{
				std::string result;
				bool isFunction = false;
				m_syntactic->GetSymbolType(node->m_value.GetLexeme(), functionName, isFunction);
				if (isFunction)
				{
					std::map<int, std::string> parameters = m_syntactic->GetFuncParameterTypes(functionName);

					for (int i = node->m_subTrees.size() - 1; i >= 0; i--)
					{
						if (i < parameters.size())
						{
							result = CreateIntermidiateCode(node->m_subTrees[i], varIndex, functionName);
							result += "STO 0, var" + to_string(i) + "\n";
						}
					}
					for (int i = node->m_subTrees.size() - 1; i >= 0; i--)
					{
						result += "LOD var" + to_string(i) + ", 0\n";
					}
					result += "CAL 0, " + node->m_value.GetLexeme() + "\n";
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
			r += CreateIntermidiateCode(node->m_left, varIndex, functionName);
			return r + "OPR 0, 15\n";
		}
		else if (node->m_value.GetLexeme() == "&&")
		{
			r += CreateIntermidiateCode(node->m_left, varIndex, functionName);
			return r + "OPR 0, 14\n";
		}
		else if (node->m_value.GetLexeme() == "==")
		{
			r += CreateIntermidiateCode(node->m_left, varIndex, functionName);
			return r + "OPR 0, 12\n";
		}
		else if (node->m_value.GetLexeme() == "!=")
		{
			r += CreateIntermidiateCode(node->m_left, varIndex, functionName);
			return r + "OPR 0, 13\n";
		}
		else if (node->m_value.GetLexeme() == "<")
		{
			r += CreateIntermidiateCode(node->m_left, varIndex, functionName);
			return r + "OPR 0, 10\n";
		}
		else if (node->m_value.GetLexeme() == "<=")
		{
			r += CreateIntermidiateCode(node->m_left, varIndex, functionName);
			return r + "OPR 0, 11\n";
		}
		else if (node->m_value.GetLexeme() == ">")
		{
			r += CreateIntermidiateCode(node->m_left, varIndex, functionName);
			return r + "OPR 0, 8\n";
		}
		else if (node->m_value.GetLexeme() == ">=")
		{
			r += CreateIntermidiateCode(node->m_left, varIndex, functionName);
			return r + "OPR 0, 9\n";
		}
		else if (node->m_value.GetLexeme() == "+")
		{
			r += CreateIntermidiateCode(node->m_left, varIndex, functionName);
			return r + "OPR 0, 2\n";
		}
		else if (node->m_value.GetLexeme() == "-" && node->m_value.GetType() == eTOKEN_TYPE::ARITHMETIC_OPERATOR)
		{
			r += CreateIntermidiateCode(node->m_left, varIndex, functionName);
			return r + "OPR 0, 3\n";
		}
		else if (node->m_value.GetLexeme() == "*")
		{
			r += CreateIntermidiateCode(node->m_left, varIndex, functionName);
			return r + "OPR 0, 4\n";
		}
		else if (node->m_value.GetLexeme() == "/")
		{
			r += CreateIntermidiateCode(node->m_left, varIndex, functionName);
			return r + "OPR 0, 5\n";
		}
		else if (node->m_value.GetLexeme() == "%")
		{
			r += CreateIntermidiateCode(node->m_left, varIndex, functionName);
			return r + "OPR 0, 6\n";
		}
		else if (node->m_value.GetLexeme() == "^")
		{
			r += CreateIntermidiateCode(node->m_left, varIndex, functionName);
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
}