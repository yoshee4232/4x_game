#pragma once
#include "physObject.h"
#include "asteroid.h"
#include "planet.h"
#include "keplarianConversions.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
using namespace std;
using namespace sf;

class star:
	public physObject
{
public:
	star(double xx, double yy, float mass1);
	~star();
	void draw(RenderWindow&, view v);
	float radius;
	planet* planets;
	asteroid* asteroids;
};

