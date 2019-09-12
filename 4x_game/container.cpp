#include "container.h"

container::container()
{
	
}

void container::setSize(int _width, int _height){width = _width; height = _height;}

void container::setPosition(int _x, int _y){localX = _x; localY = _y;}

void container::setParentLoc(int _x, int _y) { parentX = _x; parentY = _y; }

container::~container()
{

}