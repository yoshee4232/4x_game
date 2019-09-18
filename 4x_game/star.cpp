#include "star.h"


star::star(double xx, double yy, float mass1)
{
	x = xx;
	y = yy;
	mass = mass1;
}


star::~star()
{
}

void star::draw(RenderWindow& window, view v)
{
	CircleShape shape(1000000.0 * v.zoom);
	shape.setOrigin(100.0 * v.zoom / 2.0, 100.0 * v.zoom / 2.0);
	shape.setFillColor(Color::Green);
	shape.setPosition((x - v.x) * v.zoom, (y - v.y) * v.zoom);
	window.draw(shape);
}
