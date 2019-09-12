#pragma once
#include "physObject.h"
#include "keplarianConversions.h"
class planet :
	public physObject
{
public:
	planet();
	~planet();
	void setRadius(float _radius);
	void setOrbit(orbit o);
	void draw(RenderWindow&, view v);
	orbit _orbit;
private:
	float radius;
	
};

