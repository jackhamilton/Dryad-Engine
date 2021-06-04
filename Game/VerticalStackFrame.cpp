#include "VerticalStackFrame.h"

VerticalStackFrame::VerticalStackFrame(vector<shared_ptr<GameObject>> objects)
{
	for (shared_ptr<GameObject> o : objects) {
		children.push_back(o);
	}
	isVerticalStackFrame = true;
	isUIElement = true;
}

void VerticalStackFrame::align(Rectangle r)
{
	double x = r.x, y = r.y;
	int spaces = children.size() - 1;
	spaces = spaces > 0 ? spaces : 1;
	int totalHeights = 0;
	int stackFrames = 0;
	for (shared_ptr<GameObject> o : children) {
		if (!(o->isHorizontalStackFrame || o->isVerticalStackFrame)) {
			totalHeights += o->getSize().height;
		}
		else {
			stackFrames++;
		}
	}
	int sFrameSpacing = 0;
	int spacing = r.height - totalHeights;
	spacing = spacing > 0 ? spacing : 0;
	if (stackFrames > 0) {
		int compInterspacing = interspacing * spaces;
		sFrameSpacing = spacing - compInterspacing;
		spacing = compInterspacing;
	}
	spacing = spacing > 0 ? spacing : 0;
	spacing /= spaces;

	for (shared_ptr<GameObject> o : children) {
		o->setPosition(Point(r.x, r.y));
		if (o->isHorizontalStackFrame || o->isVerticalStackFrame) {
			o->size.width = r.width;
			o->size.height = sFrameSpacing / stackFrames;
			static_pointer_cast<StackFrame>(o)->align();
		}
		r.y += spacing;
		r.y += (o->size.height >= 0 ? o->size.height : 0);
	}
}

void VerticalStackFrame::align()
{
	align({ int(position.x), int(position.y), int(size.width), int(size.height) });
}