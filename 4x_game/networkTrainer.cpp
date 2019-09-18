#include "networkTrainer.h"



networkTrainer::networkTrainer()
{
	nets.resize(100);
}

void networkTrainer::trainGen()
{
	for (int i = 0; i < 100; i++)
	{
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
			control c;
			c = nets[i].getOutput(o, ship1, earth);
			ship1.performBurn(timeStep, c.thrust, c.angle);
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

		float fitness = (v * v) + sqrt(d);

		cout << "tested ship: " << i << " fitness is: " << fitness << endl;
	}
	
}

networkTrainer::~networkTrainer()
{
}
