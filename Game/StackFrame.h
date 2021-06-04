#pragma once
#include "GameObject.h"

class StackFrame : public GameObject {
public:
	virtual void align(Rectangle r) {};
	virtual void align() {};
};