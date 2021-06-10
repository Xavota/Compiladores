#pragma once
#include "LexicState.h"
#include <string>
using namespace std;

namespace Compilador
{
class IDState : public LexicState
{
public: 
	IDState(string buffer);
	~IDState();

	LexicState* NextChar(char c, int& putback, class AnalizadorLexico* lexic);
	void EndOfFile(class AnalizadorLexico* lexic);
};
}
