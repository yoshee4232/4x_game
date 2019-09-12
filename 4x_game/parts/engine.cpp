#include "engine.h"



engine::engine()
{
}

engine::engine(int _isp, int _thrust)
{
	isp = _isp;
	//isp is a stupid way of efficiency for equations
	exhaustVelocity = isp * 9.80665;
	thrust = _thrust;
}

float engine::getFlowRate() const
{
	return thrust / exhaustVelocity;
}

engine::~engine()
{
}
