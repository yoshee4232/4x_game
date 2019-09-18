#pragma once
#include "neuralNet.h"
#include "planet.h"
#include "keplarianConversions.h"
#include <iostream>
#include <vector>

class networkTrainer
{
public:
	networkTrainer();
	void trainGen();
	~networkTrainer();
	vector<neuralNet> nets;
	float orbitVariability;
	float shipVariability;
};

