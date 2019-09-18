#include "ship.h"



ship::ship()
{
	hasGravity = false;
}

float ship::calculateDv()
{
	return 0;
}

void ship::setActiveEngine(int n)
{
	activeEngine = engines[n];
	maxDeltaV = activeEngine.exhaustVelocity * log(fueledMass / emptyMass);
}

float ship::getDv() const
{
	return activeEngine.exhaustVelocity * log(mass / emptyMass);
}

void ship::setOrbit(orbit o)
{
	_orbit = o;
}

float ship::getFlowRate() const
{
	return activeEngine.getFlowRate();
}

void ship::performBurn(float time, float thrust, float angle)
{
	float fuelRequirement = time * activeEngine.getFlowRate() * (thrust / (activeEngine.thrust / mass));
	if (fuel > fuelRequirement)
	{
		vx += thrust * cos(angle) * time;
		vy += thrust * sin(angle) * time;
		fuel -= fuelRequirement;
		mass = fuel + emptyMass;
	}
	else
	{
		return;
	}

	x += vx * time;
	y += vy * time;
}

/*
double ship::getThrust()
{

}

float ship::getAcceleration()
{

}
*/
void ship::addEngine(engine e, int ammount)
{
	for(int i = 0; i < ammount; i++)
		engines.push_back(e);
}

void ship::draw(RenderWindow& window, view v)
{
	double dx = (x - v.x) * v.zoom;
	double dy = (y - v.y) * v.zoom;
	//triangle
	CircleShape shape(400000*v.zoom, 3);
	shape.setOrigin((40.0 * v.zoom) / 2.0, (40.0 * v.zoom) / 2.0);
	shape.setFillColor(Color::Blue);
	shape.setPosition(dx, dy);
	shape.setRotation((angle * 180) / pi);
	window.draw(shape);
	VertexArray quad(sf::Lines, 2);
	quad[0].position = sf::Vector2f(dx, dy);
	quad[1].position = sf::Vector2f(dx + 30 * cos(angle), dy + 30 * sin(angle));
	window.draw(quad);
}

ship::~ship()
{
	
}
