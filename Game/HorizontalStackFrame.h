#pragma once
#include "GameObject.h"
#include "Rectangle.h"
#include "StackFrame.h"
#include <vector>
#include <memory>

class HorizontalStackFrame : public StackFrame
{
public:
	HorizontalStackFrame(vector<shared_ptr<GameObject>> objects);
	void align(Rectangle r);
	void align();
private:
	int interspacing = 5;
	friend class UIBlock;
};