#pragma once
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
using namespace std;
using namespace sf;
class node
{
public:
	node();
	void draw(RenderTexture&, Font& font);
	vector<int> getMaxWidth(map<string, node>& nodeMap);
	void setPos(map<string, node>& nodeMap);
	void processNodes(map<string, node>& nodeMap);
	~node();
	vector<string> parents;
	vector<string> children;
	Vector2f pos = Vector2f(-1,-1);
	float reservedY;
	int fontSize = 14;
	const int yPadding = 20;
	const int xPadding = 75;
	const int height = 30;
	const int width = 150;
	int tier;
	//this is how wide all of our children get
	int maxWidth = -1;
	bool unlocked;
	bool isDrawn;
	string name;
	string description;
	string category;
	string partClass;
private:
	RectangleShape box;
	vector<VertexArray> connectors;
};

