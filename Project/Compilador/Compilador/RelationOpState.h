#pragma once
#include "LexicState.h"
#include <string>
using namespace std;

namespace Compilador
{
class RelationOpState : public LexicState
{
public:
	RelationOpState(string buffer);
	~RelationOpState();

	LexicState* NextChar(char c, int& putback, class AnalizadorLexico* lexic);
	void EndOfFile(class AnalizadorLexico* lexic);
};
}