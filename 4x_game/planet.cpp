#include "planet.h"



planet::planet()
{
	hasGravity = true;
}


planet::~planet()
{
}

void planet::setRadius(float _radius)
{
	radius = _radius;
}

void planet::setOrbit(orbit o)
{
	_orbit = o;
}

void planet::draw(RenderWindow& window, view v)
{
	CircleShape shape(1000000.0 * v.zoom);
	shape.setOrigin(100.0 * v.zoom / 2.0, 100.0 * v.zoom / 2.0);
	shape.setFillColor(Color::Green);
	shape.setPosition((x - v.x) * v.zoom, (y - v.y) * v.zoom);
	window.draw(shape);
}