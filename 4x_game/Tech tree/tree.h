#pragma once
#include <map>
#include "node.h"
#include <fstream>
#include <sstream>
#include <streambuf>
using namespace std;
class tree
{
public:
	tree();
	~tree();
	string getText(ifstream&);
	string getText(istringstream& f);
	void draw(RenderTexture& r, Font& font);
	map<string, node> nodes;
};

