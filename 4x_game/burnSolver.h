#pragma once
#include "keplarianConversions.h"
#include "physObject.h"
#include "ship.h"
#include "planet.h"
#include "star.h"
#include "constants.h"
#include "ceres/dynamic_autodiff_cost_function.h"
#include "ceres/ceres.h"
#include "glog/logging.h"

using ceres::NumericDiffCostFunction;
using ceres::DynamicAutoDiffCostFunction;
using ceres::CostFunction;
using ceres::Problem;
using ceres::Solver;
using ceres::Solve;

const int kStride = 10;

/*
CostFunction* cost_function
	= new AutoDiffCostFunction<MyScalarCostFunctor, 1, 2, 2>(
		new MyScalarCostFunctor(1.0));              ^  ^  ^
													|  |  |
						Dimension of residual ------+  |  |
						Dimension of x ----------------+  |
						Dimension of y -------------------+
*/
class explicitSolve {
public:
	explicitSolve(physObject& _target, ship& s, physObject& _orbitingBody, double& period) :
		target(_target), _ship(s), orbitingBody(_orbitingBody), period(period){}
	//The output is always the last parameter
	//t has 4 values, accel start, accel end, deccel start, decell end
	//burnAngle has 4, 2 for each burn, the angle delta is linear between the two target angles
	//the burn angle is relative to the orbiting body
	//the residual is the time and relative distance/velocity of target
	bool operator()(const double* const a, const double* const theta, double* cost) const {
		//all calculations done relative to orbiting body
		//this uses newtonian methods to compute
		float fuel = _ship.fuel;
		float emptyMass = _ship.emptyMass;
		float flowRate = _ship.activeEngine.getFlowRate();
		float mass = _ship.mass;
		double burnTime = fuel / flowRate;

		float rvx = (_ship.vx - orbitingBody.vx);
		float rvy = (_ship.vy - orbitingBody.vy);
		float rx = (_ship.x - orbitingBody.x);
		float ry = (_ship.y - orbitingBody.y);
		double timeStep = period / 50.0;

		for (int i = 0; i < 50; i++)
		{
			//25 steps in first burn
			double r = sqrt(rx * rx + ry * ry);
			float currentAngle = theta[i]/* + (burnAngles[1] * (t / (totalTime1)))*/;
			if (fuel > 0)
			{
				rvx += a[i] * cos(currentAngle) * timeStep;
				rvy += a[i] * sin(currentAngle) * timeStep;
				fuel -= timeStep * flowRate * (a[i] / (_ship.activeEngine.thrust / mass));
				mass = fuel + emptyMass;
			}
			
			//simulate gravity
			rvx -= ((G * orbitingBody.mass) / (r * r)) * cos(atan(rx / ry)) * timeStep;
			rvy -= ((G * orbitingBody.mass) / (r * r)) * sin(atan(rx / ry)) * timeStep;
			
			rx += rvx * timeStep;
			ry += rvy * timeStep;
		}

		Vector2d pos1 = keplarianToCartesian(cartesianToKeplarian(target.x, target.y, target.vx, target.vy, orbitingBody.mass), 10000);
		Vector2d pos2 = keplarianToCartesian(cartesianToKeplarian(target.x, target.y, target.vx, target.vy, orbitingBody.mass), 10000 + 1);
		Vector2d vt;
		//target velocity
		vt.x = pos2.x - pos1.x;
		vt.y = pos2.y - pos1.y;

		rvx = (rvx - vt.x);
		rvy = (rvy - vt.y);
		rx = (rx - pos1.x);
		ry = (ry - pos1.y);
		float v = sqrt(rvx * rvx + rvy * rvy);
		float d = sqrt(rx * rx + ry * ry);
		cost[0] = (v*v)+sqrt(d);

		return true;
	}

private:
	physObject target;
	ship _ship;
	physObject orbitingBody;
	double period;
};

class TimeFunctor {
public:
	TimeFunctor(physObject &_target, ship &s, physObject &_orbitingBody) :
		target(_target), _ship(s), orbitingBody(_orbitingBody) {}
	//The output is always the last parameter
	//t has 4 values, accel start, accel end, deccel start, decell end
	//burnAngle has 4, 2 for each burn, the angle delta is linear between the two target angles
	//the burn angle is relative to the orbiting body
	//the residual is the time and relative distance/velocity of target
	bool operator()(const double* const burnStart, const double* const burnRatio, const double* const coastTime, const double* const burnAngles, double* cost) const {
		//all calculations done relative to orbiting body
		//this uses newtonian methods to compute
		float fuel = _ship.fuel;
		float emptyMass = _ship.emptyMass;
		float flowRate = _ship.activeEngine.getFlowRate();
		float mass = _ship.mass;
		double burnTime = fuel / flowRate;
		
		//relative veloicty and position
		float rvx = (_ship.vx - orbitingBody.vx);
		float rvy = (_ship.vy - orbitingBody.vy);
		float rx = (_ship.x - orbitingBody.x);
		float ry = (_ship.y - orbitingBody.y);

		//iterations
		double t = 0;

		float startVelocity = sqrt(rvx * rvx + rvy * rvy);
		double totalTime1 = burnTime * burnRatio[0];
		double totalTime2 = burnTime * (1.0 - burnRatio[0]);
		//calculate up to time of first burn
		//fuck it, just get the difference, I can't be bothered to do math, accuracy should be high enough
		Vector2d pos1 = keplarianToCartesian(cartesianToKeplarian(rx, ry, rvx, rvy, orbitingBody.mass), burnStart[0]);
		Vector2d pos2 = keplarianToCartesian(cartesianToKeplarian(rx, ry, rvx, rvy, orbitingBody.mass), burnStart[0] + 1);
		rvx = pos2.x - pos1.x;
		rvy = pos2.y - pos1.y;
		rx = pos1.x;
		ry = pos1.y;

		//calculate first burn
		for (int i = 0; i < 25; i++)
		{
			//25 steps in first burn
			double r = sqrt(rx * rx + ry * ry);
			float currentAngle = burnAngles[0]/* + (burnAngles[1] * (t / (totalTime1)))*/;
			float dt = totalTime1 / 25.0;
			rvx += (_ship.activeEngine.thrust / mass) * cos(currentAngle) * dt;
			rvy += (_ship.activeEngine.thrust / mass) * sin(currentAngle) * dt;
			//simulate gravity
			rvx -= ((G * orbitingBody.mass) / (r * r)) * cos(atan(rx / ry)) * dt;
			rvy -= ((G * orbitingBody.mass) / (r * r)) * sin(atan(rx / ry)) * dt;
			fuel -= dt * flowRate;
			mass = fuel + emptyMass;
			t += dt;
			rx += rvx;
			ry += rvy;
		}

		//calculate to second burn
		pos1 = keplarianToCartesian(cartesianToKeplarian(rx, ry, rvx, rvy, orbitingBody.mass), coastTime[0]);
		pos2 = keplarianToCartesian(cartesianToKeplarian(rx, ry, rvx, rvy, orbitingBody.mass), coastTime[0] + 1);
		rvx = pos2.x - pos1.x;
		rvy = pos2.y - pos1.y;
		rx = pos1.x;
		ry = pos1.y;
		t += coastTime[0];

		//calculate second burn
		for (int i = 0; i < 25; i++)
		{
			//25 steps in first burn
			float dt = totalTime2 / 25.0;
			double r = sqrt(rx * rx + ry * ry);
			//fix this, it technically should work out but like, don't leave it like this
			float currentAngle = burnAngles[2] /*+ (burnAngles[3] * (t - (totalTime1 + coastTime[0]) / (totalTime2)))*/;
			rvx += (_ship.activeEngine.thrust / mass) * cos(currentAngle) * dt;
			rvy += (_ship.activeEngine.thrust / mass) * sin(currentAngle) * dt;
			//simulate gravity
			rvx -= ((G * orbitingBody.mass) / (r * r)) * cos(atan(rx / ry)) * dt;
			rvy -= ((G * orbitingBody.mass) / (r * r)) * sin(atan(rx / ry)) * dt;
			fuel -= dt * flowRate;
			mass = fuel + emptyMass;
			t += dt;
			rx += rvx;
			ry += rvy;
		}

		pos1 = keplarianToCartesian(cartesianToKeplarian(target.x, target.y, target.vx, target.vy, orbitingBody.mass), burnStart[0] + totalTime1 + coastTime[0] + totalTime2);
		pos2 = keplarianToCartesian(cartesianToKeplarian(target.x, target.y, target.vx, target.vy, orbitingBody.mass), burnStart[0] + totalTime1 + coastTime[0] + totalTime2 + 1);
		Vector2d vt;
		//target velocity
		vt.x = pos2.x - pos1.x;
		vt.y = pos2.y - pos1.y;

		rvx = (rvx - vt.x);
		rvy = (rvy - vt.y);
		rx = (rx - pos1.x);
		ry = (ry - pos1.y);
		float v = sqrt(rvx * rvx + rvy * rvy);
		float d = sqrt(rx * rx + ry * ry);
		cost[0] = (v) + sqrt(d) + (burnStart[0] + totalTime1 + coastTime[0] + totalTime2);

		return true;
	}

private:
	physObject target;
	ship _ship;
	physObject orbitingBody;
};