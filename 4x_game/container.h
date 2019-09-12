#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>
using namespace std;
using namespace sf;

class container
{
public:
	container();
	void setSize(int width, int height);
	void setPosition(int x, int y);
	void setParentLoc(int _x, int _y);
	~container();
protected:
	int parentX = 0;
	int parentY = 0;
	int localX = 0;
	int localY = 0;
	int width = 0;
	int height = 0;
};

