#pragma once
#include "LexicState.h"
#include <string>
using namespace std;

namespace Compilador
{
class LogicOpState : public LexicState
{
public:
	LogicOpState(string buffer);
	~LogicOpState();

	LexicState* NextChar(char c, int& putback, class AnalizadorLexico* lexic);
	void EndOfFile(class AnalizadorLexico* lexic);
};
}
