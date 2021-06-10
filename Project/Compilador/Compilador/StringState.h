#pragma once
#include "LexicState.h"
#include <string>
using namespace std;

namespace Compilador
{
class StringState : public LexicState
{
public:
	StringState(string buffer);
	~StringState();

	LexicState* NextChar(char c, int& putback, class AnalizadorLexico* lexic);
	void EndOfFile(class AnalizadorLexico* lexic);
};
}
