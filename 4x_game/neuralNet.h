
#pragma once
#define NETWORK_DEPTH 2
#define HIDDEN_LAYER_WIDTH 8
#define INPUTS 7
const int GENOME_SIZE = 24 * ((INPUTS + 1) * HIDDEN_LAYER_WIDTH +
(NETWORK_DEPTH - 1) * (HIDDEN_LAYER_WIDTH + 1) * HIDDEN_LAYER_WIDTH +
2 * (HIDDEN_LAYER_WIDTH + 1));
#include "keplarianConversions.h"
#include <bitset>
#include "physObject.h"
#include "ship.h"
struct control {
public:
	float angle;
	float thrust;
};

typedef union {
	int i;
	float f;
} u;

class neuralNet
{
public:
	neuralNet();
	neuralNet(neuralNet &n1, neuralNet &n2, float mutationRate);
	control getOutput(orbit target, ship _ship, physObject orbitingBody);
	~neuralNet();
	
	bitset<GENOME_SIZE> genome;
	float inputs[INPUTS];
	float outputs[2];
	float nodeValues1[HIDDEN_LAYER_WIDTH];
	float nodeValues2[HIDDEN_LAYER_WIDTH];
private:
	float getGene(int loc);
};

