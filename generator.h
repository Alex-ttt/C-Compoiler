#pragma once

#include "commands.h"
#include <string>

using namespace std;

class CodeGenerator{
private:
	string file;
	AsmCode data;
	AsmCode code;

public:
	friend class Parser;
	CodeGenerator(const string &name) : file(name) {}
	void generate();
};