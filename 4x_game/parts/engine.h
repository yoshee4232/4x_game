#pragma once
#include "part.h"
class engine:
	public part
{
public:
	engine();
	engine(int _isp, int _thrust);
	float getFlowRate() const;
	~engine();
	int isp = 0;
	//in newtons
	float thrust = 100;
	float exhaustVelocity = 3000;
};

