#include "pane.h"



pane::pane(int _width, int _height, int _x, int _y)
{
	width = _width;
	height = _height;
	x = _x;
	y = _y;
	//draggable bar
	bar.setSize(Vector2f(width - 10, 20));
	bar.setPosition(x + 5, y + 5);
	//main window area
	main.setSize(Vector2f(width, height));
	main.setPosition(x, y);
	main.setFillColor(Color(100, 100, 100, 255));
	//
	exit.setSize(Vector2f(20, 16));
	exit.setPosition(x+7, y+7);
	exit.setFillColor(Color(255, 0, 0, 255));
}


pane::~pane()
{
}

void pane::addContent(textBox _textBox)
{
	textBoxVect.push_back(_textBox);
}

bool pane::checkMouseCollision(RenderWindow& window)
{
	return bar.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y);
}

//bool returns if the window is being moved
void pane::draw(RenderTexture& gui, Font font)
{
	bar.setPosition(x + 5, y + 5);
	exit.setPosition(x + 7, y + 7);
	main.setPosition(x, y);
	//draggable code
	gui.draw(main);
	gui.draw(bar);
	gui.draw(exit);

	//render all text boxes
	for (int i = 0; i < textBoxVect.size(); i++)
	{
		textBoxVect[i].setParentLoc(x, y);
		textBoxVect[i].draw(gui, font);
	}
}
