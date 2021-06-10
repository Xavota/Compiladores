#pragma once
#include "LexicState.h"
#include <string>
using namespace std;


namespace Compilador
{
class IntegerState : public LexicState
{
public:
	IntegerState(string buffer);
	~IntegerState();

	LexicState* NextChar(char c, int& putback, class AnalizadorLexico* lexic);
	void EndOfFile(class AnalizadorLexico* lexic);
};
}
