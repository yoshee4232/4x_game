#include "neuralNet.h"



neuralNet::neuralNet()
{
	for (int i = 0; i < GENOME_SIZE; i++)
	{
		genome[i] = rand() % 2;
	}
}

neuralNet::neuralNet(neuralNet &n1, neuralNet &n2, float mutationRate)
{
	int split = rand() % GENOME_SIZE;
	for (int i = 0; i < split; i++)
	{
		if (rand() < mutationRate)
			genome[i] = rand() % 2;
		else
			genome[i] = n1.genome[i];
	}

	for (int i = split; i < GENOME_SIZE; i++)
	{
		if (rand() < mutationRate)
			genome[i] = rand() % 2;
		else
			genome[i] = n1.genome[i];
	}
}

control neuralNet::getOutput(orbit target, ship _ship, physObject orbitingBody)
{
	//initial definitions****************************************************************************************************************************************************
	//this returns an angle relative to the current velocity to thrust in
	Vector2d rt = keplarianToCartesian(target, 0);
	Vector2d rs = keplarianToCartesian(_ship._orbit, 0);
	Vector2d vt = getVelocity(target, 0);
	Vector2d vs = getVelocity(_ship._orbit, 0);
	Vector2d rRelative;
	rRelative.x = rt.x - rs.x;
	rRelative.y = rt.y - rs.y;
	double d = sqrt(rRelative.x * rRelative.x + rRelative.y * rRelative.y);
	Vector2d vRelative;
	vRelative.x = vt.x - vs.x;
	vRelative.y = vt.y - vs.y;
	double relativeTheta = atan2(rRelative.y, rRelative.x);
	double velocityTheta = atan2(vRelative.y, vRelative.x);

	double shipTheta = atan2(rs.y, rs.x);
	double targetTheta = atan2(rt.y, rt.x);
	
	//this is the first input
	double seperationAngle = (shipTheta - targetTheta) / (pi);
	//next is velocity engle relative to the vector pointing twoards the target
	double velocityAngle = (relativeTheta - velocityTheta) / (pi);
	//delta range from -1 - 1
	double deltaV = -1.0 + (_ship.getDv() / _ship.maxDeltaV) * 2.0;
	//range from -1 - 1
	double acceleration = -1 + 2.0 / (sqrt(_ship.activeEngine.thrust / _ship.mass) + 1);
	//range from -1 - 1
	double fuelTime = -1.0 + (min((2.0 * max(target.period, _ship._orbit.period) / (_ship.fuel / _ship.getFlowRate())), 2.0));
	//range from 0 - 1 
	double distance = d / max(target.semiMajorAxis, _ship._orbit.semiMajorAxis);
	//percentage of ship that is fuel, probably important
	double fuelPercent = _ship.fueledMass / _ship.emptyMass;

	control c;
	c.angle = 0;
	c.thrust = 0;
	//initial definitions****************************************************************************************************************************************************
	//actual calculations, feed forward net
	inputs[0] = seperationAngle;
	inputs[1] = velocityAngle;
	inputs[2] = deltaV;
	inputs[3] = acceleration;
	inputs[4] = fuelTime;
	inputs[5] = distance;
	inputs[6] = fuelPercent;

	int offset = 0;

	for (int i = 0; i < HIDDEN_LAYER_WIDTH; i++)
	{
		nodeValues1[i] = 0;
		for (int j = 0; j < INPUTS; j++)
		{
			nodeValues1[i] += inputs[j] * getGene(offset);
			offset++;
		}
		//calculate the biases
		//tanh function
		//nodeValues1[i] = tanh((nodeValues1[i] / static_cast<double>(HIDDEN_LAYER_WIDTH) + getGene(offset)));
		nodeValues1[i] = max(max(0.0, (nodeValues1[i] / static_cast<double>(HIDDEN_LAYER_WIDTH) + getGene(offset))), 1.0);
		offset++;
	}

	for (int i = 0; i < (NETWORK_DEPTH - 1); i++)
	{
		
		for (int j = 0; j < HIDDEN_LAYER_WIDTH; j++)
		{
			nodeValues2[j] = 0;
			for (int k = 0; k < HIDDEN_LAYER_WIDTH; k++)
			{
				nodeValues2[j] += nodeValues1[k] * getGene(offset);
				offset++;
			}
			//nodeValues1[j] = tanh((nodeValues1[j] / static_cast<double>(HIDDEN_LAYER_WIDTH) + getGene(offset)));
			nodeValues1[j] = max(max(0.0, (nodeValues1[j] / static_cast<double>(HIDDEN_LAYER_WIDTH) + getGene(offset))), 1.0);
			offset++;
		}
	}

	for (int i = 0; i < HIDDEN_LAYER_WIDTH; i++)
	{
		nodeValues1[0] += nodeValues2[i] * getGene(offset);
		offset++;
	}
	c.angle = tanh((nodeValues1[0] / static_cast<double>(HIDDEN_LAYER_WIDTH)) + getGene(offset)) * pi;

	for (int i = 0; i < HIDDEN_LAYER_WIDTH; i++)
	{
		nodeValues1[1] += nodeValues2[i] * getGene(offset);
		offset++;
	}
	c.thrust = .5 + (tanh((nodeValues1[1] / static_cast<double>(HIDDEN_LAYER_WIDTH)) + getGene(offset)) / 2.0);

	offset++;


	return c;
}

float neuralNet::getGene(int loc)
{
	int bitwise = 0x00000000;
	bitwise |= genome[loc * 24] << 31;
	for (int i = 0; i < 23; i++)
	{
		bitwise |= genome[loc * 24 + i + 1] << i;
	}
	u u1;
	u1.f = 1.0;
	u1.i |= bitwise;
	u1.f = u1.f/2.0;
	return u1.f;
}

neuralNet::~neuralNet()
{
}
