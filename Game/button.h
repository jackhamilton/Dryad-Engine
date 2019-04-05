#pragma once
#include <list>

class Sprite;

class button
{
public:
	button(const char* text);
	~button();
private:
	std::list<Sprite> buttonStates;
};

