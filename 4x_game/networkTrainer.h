#pragma once
#define POPULATION_SIZE 500
#include "planet.h"
#include "keplarianConversions.h"
#include <thread>
#include <iostream>
#include <vector>
#include <future>
#include "neuralNet.h"

class networkTrainer
{
public:
	networkTrainer();
	void trainGen();
	~networkTrainer();
	vector<neuralNet> nets;
	thread threads[POPULATION_SIZE];
	future<float> fitnesses[POPULATION_SIZE];
	float orbitVariability;
	float shipVariability;
};

