#include "tree.h"

tree::tree()
{
	ifstream f;
	f.open("Tech tree/tech_tree.txt");
	vector<string> t;
	string word;
	string prop;
	while (f >> word)
	{
		char c;
		f.get(c);
		//check for comments
		if (c == '#')
		{
			getline(f, prop);
		}
		else if (word == "node")
		{
			f.unget();
			node n;
			n.name = getText(f);
			f.get(c);
			while (c != '{')
				f.get(c);
			getline(f, prop, '}');

			istringstream p(prop);
			string num;

			while (p >> word)
			{
				if (word == "tier")
				{
					p >> num;
					n.tier = stoi(num);
				}
				if (word == "description")
					n.description = getText(p);
				if (word == "unlockedAtStart")
					n.unlocked = true;
				if (word == "category")
					n.category = getText(p);
				if (word == "partClass")
					n.partClass = getText(p);
				//we will set the children after we have all the nodes
				if (word == "requires")
					n.parents.push_back(getText(p));
					
			}
			nodes.insert(pair<string, node>(n.name, n));
		}
	}
	//now set the children in place
	for (map<string, node>::iterator it = nodes.begin(); it != nodes.end(); ++it)
	{
		for (int i = 0; i < it->second.parents.size(); i++)
		{
			nodes[it->second.parents[i]].children.push_back(it->first);
		}
	}
	nodes["start"].pos.x = 50;
	nodes["start"].pos.y = 100;
	nodes["start"].processNodes(nodes);
	int it = 0;
}

string tree::getText(ifstream& f)
{
	string word = "";
	char c;
	f.get(c);
	while (c != '\"')
		f.get(c);
	f.get(c);
	while (c != '\"')
	{
		word += c;
		f.get(c);
	}
		
	return word;
}

string tree::getText(istringstream& f)
{
	string word = "";
	char c;
	f.get(c);
	while (c != '\"')
		f.get(c);
	f.get(c);
	while (c != '\"')
	{
		word += c;
		f.get(c);
	}

	return word;
}

void tree::draw(RenderTexture& r, Font& font)
{
	for (map<string, node>::iterator it = nodes.begin(); it != nodes.end(); ++it)
	{
		it->second.draw(r, font);
	}
}

tree::~tree()
{
}
