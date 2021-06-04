#pragma once
#include "GameObject.h"
#include "Rectangle.h"
#include "StackFrame.h"
#include <vector>
#include <memory>

class VerticalStackFrame : public StackFrame
{
public:
	VerticalStackFrame(vector<shared_ptr<GameObject>> objects);
	void align(Rectangle r);
	void align();
private:
	int interspacing = 5;
	friend class UIBlock;
};
