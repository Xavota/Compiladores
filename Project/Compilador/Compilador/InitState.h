#pragma once
#include "LexicState.h"
#include <string>
using namespace std;


namespace Compilador
{
class InitState : public LexicState
{
public:
	InitState(string buffer);
	~InitState();

	LexicState* NextChar(char c, int& putback, class AnalizadorLexico* lexic);
	void EndOfFile(class AnalizadorLexico* lexic);
};
}
