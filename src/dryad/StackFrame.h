#pragma once
#include "GameObject.h"

namespace dryad
{

class StackFrame : public GameObject {
public:
	virtual void align(Rectangle r) {};
	virtual void align() {};
};

}