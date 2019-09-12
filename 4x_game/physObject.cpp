#include "physObject.h"


physObject::physObject()
{
}

void physObject::setPosition(double xx, double yy)
{
	x = xx;
	y = yy;
}

void physObject::setMass(float _mass)
{
	mass = _mass;
}

void physObject::setVelocity(double vxx, double vyy)
{
	vx = vxx;
	vy = vyy;
}

physObject::~physObject()
{
}
