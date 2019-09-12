#include "node.h"



node::node()
{
}

void node::draw(RenderTexture& r, Font& font)
{
	if(category == "engine")
		box.setOutlineColor(Color(255, 157, 0,255));
	else if (category == "power")
		box.setOutlineColor(Color::Blue);
	box.setOutlineThickness(2.f);
	box.setPosition(Vector2f(pos.x, pos.y));
	box.setSize(Vector2f(width, height));
	r.draw(box);
	Text txt;
	txt.setPosition(pos.x, pos.y);
	txt.setFillColor(Color::Black);
	txt.setFont(font);
	txt.setString(name);
	txt.setCharacterSize(fontSize);
	for (int i = 0; i < connectors.size(); i++)
	{
		r.draw(connectors[i]);
	}
	r.draw(txt);

}


vector<int> node::getMaxWidth(map<string, node>& nodeMap)
{
	//this function is horribly unoptomized but it should only need to be run once

	//make sure we are right after the calling node, otherwise return 0
	vector<int> localTierCounts;
	//this just accounts for up to 100 tiers
	localTierCounts.assign(100, 0);

	maxWidth = 1;

	for (int i = 0; i < children.size(); i++)
	{
		//see if the node has already been visited
		//if not, run the iterative function
		vector<int> counts;
		counts.assign(100, 0);
		if (nodeMap[children[i]].maxWidth == -1)
		{
			counts = nodeMap[children[i]].getMaxWidth(nodeMap);
		}
		for (int n = 0; n < 100; n++)
		{
			localTierCounts[n] += counts[n];
		}
	}
	

	for (int i = 0; i < localTierCounts.size(); i++)
	{
		if (maxWidth < localTierCounts[i])
			maxWidth = localTierCounts[i];
	}

	localTierCounts[tier] += 1;

	return localTierCounts;
}

void node::setPos(map<string, node>& nodeMap)
{
	pos.x = tier * (width + xPadding);
	int slotsReserved = 0;
	
	for (int i = 0; i < children.size(); i++)
	{
		if (nodeMap[children[i]].tier == (tier + 1))
		{
			if (nodeMap[children[i]].pos.x == -1)
			{
				int maxY = maxWidth * (height + yPadding);
				nodeMap[children[i]].reservedY = pos.y - (((height + yPadding) * maxWidth) / 2.0) + slotsReserved * (height + yPadding);
				nodeMap[children[i]].pos.y = nodeMap[children[i]].reservedY + maxY / 2;
				slotsReserved += nodeMap[children[i]].maxWidth;
				nodeMap[children[i]].setPos(nodeMap);
			}
			VertexArray quad(sf::Lines, 2);
			quad[0].position = sf::Vector2f(pos.x + width, pos.y + height / 2.0);
			quad[1].position = sf::Vector2f(nodeMap[children[i]].pos.x, nodeMap[children[i]].pos.y + nodeMap[children[i]].height / 2.0);
			connectors.push_back(quad);
		}
		else
		{
			if (nodeMap[children[i]].pos.x == -1)
			{
				int maxY = maxWidth * (height + yPadding);
				nodeMap[children[i]].reservedY = pos.y - (((height + yPadding) * maxWidth) / 2.0) + slotsReserved * (height + yPadding);
				nodeMap[children[i]].pos.y = nodeMap[children[i]].reservedY + maxY / 2;
				slotsReserved += nodeMap[children[i]].maxWidth;
				nodeMap[children[i]].setPos(nodeMap);
			}
			VertexArray quad(sf::Lines, 2);
			quad[0].position = sf::Vector2f(pos.x + width, pos.y + height / 2.0);
			quad[1].position = sf::Vector2f(pos.x + width + xPadding, pos.y + height + yPadding / 2.0);
			connectors.push_back(quad);
			quad[0].position = quad[1].position;
			quad[1].position = sf::Vector2f(nodeMap[children[i]].tier * (width + xPadding) - xPadding, pos.y + height + yPadding / 2.0);
			connectors.push_back(quad);
			quad[0].position = quad[1].position;
			quad[1].position = sf::Vector2f(nodeMap[children[i]].pos.x, nodeMap[children[i]].pos.y + nodeMap[children[i]].height / 2.0);
			connectors.push_back(quad);
		}
	}
}

void node::processNodes(map<string, node>& nodeMap)
{
	getMaxWidth(nodeMap);
	
	setPos(nodeMap);

	int n = 0;
}

node::~node()
{
}
