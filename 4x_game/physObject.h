#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "structs.h"
using namespace std;
using namespace sf;

class physObject
{
public:
	physObject();
	void setPosition(double x, double y);
	void setVelocity(double vx, double vy);
	void setMass(float _mass);
	~physObject();
	bool hasGravity;
	float mass;
	double x = 0;
	double y = 0;
	double vx = 0;
	double vy = 0;
};



