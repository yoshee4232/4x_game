#include "keplarianConversions.h"

orbit cartesianToKeplarian(double x, double y, double vx, double vy, double m)
{
	//determined from this https://downloads.rene-schwarz.com/download/M002-Cartesian_State_Vectors_to_Keplerian_Orbit_Elements.pdf
	orbit out;

	Vector2d eVector;
	y = -y;
	vy = -vy;

	double radius = sqrt(x * x + y * y);
	double speed = sqrt(vx * vx + vy * vy);
	//technically speaking this is the third dimention in a 3d vector
	double h = x * vy - y * vx;
	//trust me, lots of math condensed, this calculates eccentricity vector
	eVector.x = (vy * h) / (G * m) - (x / radius);
	eVector.y = (vx * h * -1.0) / (G * m) - (y / radius);

	out.eccentricity = sqrt(eVector.x * eVector.x + eVector.y * eVector.y);

	double rv = vx * x + vy * y;
	double er = eVector.x * x + eVector.y * y;

	if (rv >= 0)
	{
		//equation is arccos(<e,r>)/(|e||r|)
		out.trueAnomaly = acos(er / (out.eccentricity * radius));
	}
	else
	{
		out.trueAnomaly = 2.0 * pi - acos(er / (out.eccentricity * radius));
	}

	out.argOfPeriapsis = atan2(eVector.y, eVector.x);

	//eccentric anomaly
	//out.eccentricAnomaly = 2.0 * atan(tan(out.trueAnomaly / 2.0))/ sqrt((1.0 + out.eccentricity) / (1.0 - out.eccentricity));

	//kepler equation
	//out.meanAnomaly = out.eccentricAnomaly - out.eccentricity * sin(out.eccentricAnomaly);

	//semi-major axis
	out.semiMajorAxis = 1.0 / ((2.0 / radius) - ((speed * speed) / (G * m)));

	out.semiMinorAxis = out.semiMajorAxis * sqrt(1 - out.eccentricity * out.eccentricity);

	out.eccentricAnomaly = acos((out.eccentricity + cos(out.trueAnomaly)) / (1 + out.eccentricity*cos(out.trueAnomaly)));

	//out.eccentricAnomaly = acos(x/out.semiMinorAxis);

	out.meanAnomaly = out.eccentricAnomaly - out.eccentricity * sin(out.eccentricAnomaly);

	out.period = 2.0 * pi * sqrt(pow(out.semiMajorAxis, 3) / (G * m));

	//if y and x have the same sign it is a clockwise orbit
	if (((vx * cos(out.argOfPeriapsis) - vy * sin(out.argOfPeriapsis)) * (vy * cos(out.argOfPeriapsis) - vx * sin(out.argOfPeriapsis))) > 0)
	{
		out.retrograde = false;
	}
	else
	{
		out.retrograde = true;
	}

	return out;
}

Vector2d keplarianToCartesian(orbit o, double t)
{
	Vector2d out;
	//mean Anomaly at given time

	//M = n(t - t0)
	
	if (o.retrograde)
	{
		t = -t;
	}
	
	double n = (2.0 * pi) / o.period;
	t = t + (o.meanAnomaly / (2.0 * pi)) * o.period;
	double M = n * t;
	double E = M;
	//approximate solution to keplers equation
	for (int i = 0; i < 10; i++)
	{
		E = M + o.eccentricity * sin(E);
	}
	
	//double r = o.semiMajorAxis * (1 - o.eccentricity * cos(o.eccentricAnomaly));

	double x = o.semiMajorAxis * (cos(E) - o.eccentricity);
	double y = o.semiMinorAxis * sin(E);

	double sint = sin(o.argOfPeriapsis);
	double cost = cos(o.argOfPeriapsis);

	out.x = x * cos(o.argOfPeriapsis) - y * sin(o.argOfPeriapsis);
	out.y = x * sin(o.argOfPeriapsis) + y * cos(o.argOfPeriapsis);
	
	out.y = -out.y;
	
	return out;
}
