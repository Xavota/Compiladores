#pragma once
#include "LexicState.h"
#include <string>
using namespace std;

namespace Compilador
{
class CharState : public LexicState
{
public:
	CharState(string buffer);
	~CharState();

	LexicState* NextChar(char c, int& putback, class AnalizadorLexico* lexic);
	void EndOfFile(class AnalizadorLexico* lexic);
};
}
