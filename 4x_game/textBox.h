#pragma once
#include "container.h"

class textBox : public container
{
public:
	textBox();
	textBox(string _text);
	void draw(RenderTexture& gui, Font& font);
	void setFontSize(int size);
	~textBox();
private:
	string text = "test";
	int fontSize = 14;
};

