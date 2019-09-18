#pragma once

#include <SFML/Window.hpp>
#include "constants.h"

struct orbit {
	double eccentricity;
	double period;
	//not required for 2d
	double eccentricAnomaly;
	double trueAnomaly;
	double meanAnomaly;
	double semiMajorAxis;
	double semiMinorAxis;
	double argOfPeriapsis;
	bool retrograde;
};

//double vector;
struct Vector2d
{
	double x;
	double y;
};

orbit cartesianToKeplarian(double x, double y, double vx, double vy, double m);

Vector2d keplarianToCartesian(orbit o, double t);

Vector2d getVelocity(orbit o, double t);

orbit orbitFromValues(double _semiMajorAxis, double _eccentricity, double trueAnomaly, double argOfPeriapsis, double m);