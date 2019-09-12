#include "textBox.h"



textBox::textBox()
{

}

textBox::textBox(string _text)
{
	text = _text;
}

textBox::~textBox()
{
}

void textBox::setFontSize(int size)
{
	fontSize = size;
}

void textBox::draw(RenderTexture& gui, Font& font)
{
	int x = parentX + localX;
	int y = parentY + localY;
	RectangleShape rect;
	rect.setSize(Vector2f(width, height));
	rect.setOutlineColor(Color(25, 25, 25));
	rect.setOutlineThickness(1.0);
	rect.setFillColor(Color(0, 0, 0, 0));
	rect.setPosition(x, y);
	Text txt;
	txt.setPosition(x, y);
	txt.setFont(font);
	txt.setString(text);
	txt.setCharacterSize(fontSize);
	gui.draw(txt);
	gui.draw(rect);
}