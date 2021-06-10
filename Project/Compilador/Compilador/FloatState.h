#pragma once
#include "LexicState.h"
#include <string>
using namespace std;

namespace Compilador
{
class FloatState : public LexicState
{
public:
	FloatState(string buffer);
	~FloatState();

	LexicState* NextChar(char c, int& putback, class AnalizadorLexico* lexic);
	void EndOfFile(class AnalizadorLexico* lexic);
};
}
