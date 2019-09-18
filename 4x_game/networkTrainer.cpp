#include "networkTrainer.h"

float f(neuralNet net) {
	planet earth;
	earth.setMass(6.1272e24f);
	earth.setPosition(0, 0);

	planet target;
	orbit o = (cartesianToKeplarian(4.1, 3e7f, -3400, 4.0, earth.mass));

	target.setOrbit(o);
	target.setPosition(4.1, 3e7f);
	target.setVelocity(-3400, 4.0);

	ship ship1;
	ship1.setOrbit(cartesianToKeplarian(2e7f, 1.1, 1.1, -4400, earth.mass));
	ship1.setPosition(2e7f, 1.1);
	ship1.setVelocity(1.1, -4400);
	engine e;
	ship1.setMass(2000);
	ship1.fuel = 1000;
	ship1.emptyMass = 1000;
	ship1.fueledMass = 2000;
	ship1.addEngine(e, 1);
	ship1.setActiveEngine(0);

	float timeStep = target._orbit.period / 100.0;

	for (int k = 0; k < 200; k++)
	{
		Vector2d targetR = keplarianToCartesian(target._orbit, timeStep * k);

		Vector2d relativeR;

		relativeR.x = ship1.x - targetR.x;
		relativeR.y = ship1.y - targetR.y;

		float targetAngle = atan2(relativeR.y, relativeR.x);

		control c;
		c = net.getOutput(o, ship1, earth);
		ship1.performBurn(timeStep, c.thrust, c.angle + targetAngle);
		double r = sqrt(ship1.x * ship1.x + ship1.y * ship1.y);
		ship1.vx -= ((G * target.mass) / (r * r)) * cos(atan(ship1.x / ship1.y)) * timeStep;
		ship1.vy -= ((G * target.mass) / (r * r)) * sin(atan(ship1.x / ship1.y)) * timeStep;
	}
	Vector2d targetR = keplarianToCartesian(target._orbit, target._orbit.period * 2.0);
	Vector2d targetV = getVelocity(target._orbit, target._orbit.period * 2.0);

	Vector2d relativeR;

	relativeR.x = ship1.x - targetR.x;
	relativeR.y = ship1.y - targetR.y;

	Vector2d relativeV;

	relativeV.x = ship1.vx - targetV.x;
	relativeV.y = ship1.vy - targetV.y;

	float v = sqrt(relativeV.x * relativeV.x + relativeV.y * relativeV.y);
	float d = sqrt(relativeR.x * relativeR.x + relativeR.y * relativeR.y);

	float fitness = ((v * v) + sqrt(d)) / 1000.0;
	
	return fitness;
}

networkTrainer::networkTrainer()
{
	/**/
	nets.resize(POPULATION_SIZE);

	for (int i = 0; i < POPULATION_SIZE; i++)
	{
		fitnesses[i] = async(f, nets[i]);
	}
	for (int i = 0; i < POPULATION_SIZE; i++)
	{
		if (fitnesses[i].valid())
		{
			cout << "tested ship, fitness is: " << fitnesses[i].get() << endl;
		}
	}
}

void networkTrainer::trainGen()
{
	
}

networkTrainer::~networkTrainer()
{
	
}