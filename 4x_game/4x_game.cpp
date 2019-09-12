
#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "ceres\ceres.h"
#include <vector>
#include <iostream>
#include "physObject.h"
#include "pane.h"
#include "planet.h"
#include "star.h"
#include "ship.h"
#include "burnSolver.h"
#include "Tech tree/tree.h"

#include "glog/logging.h"

using ceres::NumericDiffCostFunction;
using ceres::CostFunction;
using ceres::Problem;
using ceres::Solver;
using ceres::Solve;
using ceres::CENTRAL;
using ceres::FORWARD;
using ceres::RIDDERS;

using namespace std;
using namespace sf;
int mouseLastX = Mouse::getPosition().x;
int mouseLastY = Mouse::getPosition().y;
int mouseDeltaX = 0;
int mouseDeltaY = 0;
//keeps track of gui window intial location before we start dragging it
int guiIinitialX = 0;
int guiIinitialY = 0;
int mouseInitialX = 0;
int mouseInitialY = 0;
//keeps track of if the user is currently moving a window
bool dragging;
int dragIndex;
bool mouseWasPressed = false;

void drawRoutine(RenderWindow&, view);
//returns true if the user controlled the window
bool processWindow(RenderWindow& window, pane&);

int main()
{
	tree tr;
	//keplarianToCartesian(o, 1);
	Font mainFont;
	if (!mainFont.loadFromFile("Helvetica.ttf"))
	{
		cout << "error loading font file!";
	}
	
	RenderTexture gui;
	gui.create(1600, 900);

	RenderWindow window(VideoMode(1600, 900), "space!");

	View v1(Vector2f(0, 0), Vector2f(1600, 900));
	window.setView(v1);

	view v;
	
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
	ship1.addEngine(e, 1);
	ship1.setActiveEngine(0);
	
	double timeStep = ship1._orbit.period / 50.0;
	double *angles = new double[50];
	double *thrust = new double[50];
	
	double cost = 0.0;

	for (int i = 0; i < 50; i++)
	{
		angles[i] = -pi / 2;
		thrust[i] = (ship1.activeEngine.thrust / ship1.mass);
	}

	Problem p;

	p.AddResidualBlock(
		new NumericDiffCostFunction< explicitSolve, RIDDERS, 1, 50, 50>(
			new explicitSolve(target, ship1, earth, ship1._orbit.period)), NULL, thrust, angles
			);
	
	for (int i = 0; i < 30; i++)
	{
		p.SetParameterLowerBound(angles, i, -2.0 * pi);
		p.SetParameterUpperBound(angles, i, 2.0 * pi);
		p.SetParameterUpperBound(thrust, i, (ship1.activeEngine.thrust / ship1.mass));
		p.SetParameterLowerBound(thrust, i, 0);
	}
	

	Solver::Options options;
	options.max_num_iterations = 10;
	options.linear_solver_type = ceres::ITERATIVE_SCHUR;
	options.minimizer_progress_to_stdout = true;
	
	

	

	Solver::Summary summary;
	Solve(options, &p, &summary);

	for (int i = 0; i < 30; i++)
	{
		cout << angles[i] << endl;
		cout << thrust[i] << endl;
	}
	std::cout << summary.BriefReport() << "\n";
	

	vector<pane> windows;
	
	pane tmp(200, 200, 400, 100);
	textBox text("test1");
	text.setPosition(10, 30);
	text.setSize(100, 16);
	tmp.addContent(text);
	windows.push_back(tmp);
	windows.push_back(pane(200, 200, 100, 100));
	int it = 0;
	int t = 0;

	while (window.isOpen())
	{
		//mouse movement
		int mouseDeltaX = (mouseLastX - Mouse::getPosition().x);
		int mouseDeltaY = (mouseLastY - Mouse::getPosition().y);
		mouseLastX = Mouse::getPosition().x;
		mouseLastY = Mouse::getPosition().y;
		
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::MouseWheelMoved)
			{
				v.zoom += v.zoom*event.mouseWheel.delta*0.3;
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			t -= 5;
		}
		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			t += 5;
			//25 steps in first burn
			
			double r = sqrt(ship1.x * ship1.x + ship1.y * ship1.y);
			float currentAngle = angles[int(t / timeStep)] + (angles[1] * (t / (target._orbit.period)));

				ship1.vx += angles[int(t/(timeStep))] * cos(currentAngle) * timeStep;
				ship1.vy += angles[int(t / (timeStep))] * sin(currentAngle) * timeStep;

			//simulate gravity
			//ship1.vx -= ((G * target.mass) / (r * r)) * cos(atan(ship1.x / ship1.y)) * timeStep;
			//ship1.vy -= ((G * target.mass) / (r * r)) * sin(atan(ship1.x / ship1.y)) * timeStep;
			//fuel -= timeStep * flowRate * (a[0] / (_ship.activeEngine.thrust / mass));
			//mass = fuel + emptyMass;
			//ship1.x += ship1.vx * timeStep;
			//ship1.y += ship1.vy * timeStep;
		}

		ship1.angle = angles[int(t / timeStep)];
		//ship1.setPosition


		window.clear();
		gui.clear(Color(0, 0, 0, 0));

		Vector2d vec;

		vec = keplarianToCartesian(ship1._orbit, t);

		//ship1.setPosition(vec.x / 6000.f, vec.y / 6000.f);

		vec = keplarianToCartesian(target._orbit, t);

		target.setPosition(vec.x / 6000.f, vec.y / 6000.f);

		ship1.draw(window, v);

		target.draw(window, v);

		earth.draw(window, v);

		
		//window dragging routine
		
		//if we are dragging a window we only care about that
		if (dragging)
		{
			processWindow(window, windows[dragIndex]);
		}
		else   
		{
			for (int i = 0; i < windows.size(); i++)
			{
				//stop processing windows once we control one
				if (processWindow(window, windows[i]))
				{
					dragIndex = i;
					break;
				}
					
				//if it has reached the last window and hasn't moved anything, move the view
				if (Mouse::isButtonPressed(Mouse::Left) && (i == (windows.size() - 1)))
				{
					dragIndex = 10000;
					v.x += mouseDeltaX / v.zoom;
					v.y += mouseDeltaY / v.zoom;
				}
			}
		}
		mouseWasPressed = Mouse::isButtonPressed(Mouse::Left);

		for (int i = 0; i < windows.size(); i++)
		{
			windows[i].draw(gui, mainFont);
		}
		//tr.draw(gui, mainFont);-------------------------------------------------------------------------------------------------
		//draw gui layer
		gui.display();
		Sprite guiSpr(gui.getTexture());
		guiSpr.setPosition(Vector2f(-(window.getSize().x / 2.0), -(window.getSize().y / 2.0)));
		window.draw(guiSpr);
		
		window.display();
		
		
		
	}

	return 0;
}

void drawRoutine(RenderWindow& window, view v)
{
	window.clear();
	window.display();
}

bool processWindow(RenderWindow& window, pane& guiWindow)
{
	bool moved = false;
	if (dragging)
	{
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			guiWindow.x = guiIinitialX + (Mouse::getPosition().x - mouseInitialX);
			guiWindow.y = guiIinitialY + (Mouse::getPosition().y - mouseInitialY);
			moved = true;
		}
		else
		{
			dragging = false;
		}
	}
	else if (guiWindow.checkMouseCollision(window) && Mouse::isButtonPressed(Mouse::Left) && !mouseWasPressed)
	{
		guiIinitialX = guiWindow.x;
		guiIinitialY = guiWindow.y;
		mouseInitialX = Mouse::getPosition().x;
		mouseInitialY = Mouse::getPosition().y;
		moved = true;
		dragging = true;
	}
	else
		dragging = false;
	return moved;
}
