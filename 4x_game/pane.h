#pragma once
#include "container.h"
#include "textBox.h"
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
using namespace sf;

class pane
{
public:
	pane(int _width, int _height, int x, int y);
	~pane();
	void draw(RenderTexture& gui, Font font);
	bool checkMouseCollision(RenderWindow& window);
	void addContent(textBox _textBox);
	int width;
	int height;
	int x;
	int y;
	bool wasDragged = false;
private:
	//shapes that make up the window
	RectangleShape bar;
	RectangleShape main;
	RectangleShape exit;
	vector<textBox> textBoxVect;
};

