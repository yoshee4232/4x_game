#pragma once

#include "physObject.h"
#include "parts\engine.h"
#include "keplarianConversions.h"
class ship :
	public physObject
{
public:
	ship();
	void setActiveEngine(int n);
	/*
	double getThrust();
	float getAcceleration();
	*/
	float getFlowRate() const;
	void addEngine(engine e, int ammount);
	void draw(RenderWindow& r, view v);
	void setOrbit(orbit o);
	float calculateDv();
	~ship();
	float emptyMass;
	float fueledMass;
	float fuel;
	float deltaV;
	float angle = 0;
	engine activeEngine;
	vector<engine> engines;
	orbit _orbit;
};

