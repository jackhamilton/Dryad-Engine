#include "HorizontalStackFrame.h"

namespace dryad
{

HorizontalStackFrame::HorizontalStackFrame(std::vector<std::shared_ptr<GameObject>> objects)
{
	for (std::shared_ptr<GameObject> o : objects) {
		children.push_back(o);
	}
	isHorizontalStackFrame = true;
	isUIElement = true;
}

void HorizontalStackFrame::align(Rectangle r)
{
	double x = r.x, y = r.y;
	int spaces = children.size() - 1;
	spaces = spaces > 0 ? spaces : 1;
	int totalWidths = 0;
	int stackFrames = 0;
	for (std::shared_ptr<GameObject> o : children) {
		if (!(o->isHorizontalStackFrame || o->isVerticalStackFrame)) {
			totalWidths += o->getSize().width;
		}
		else {
			stackFrames++;
		}
	}
	int sFrameSpacing = 0;
	int spacing = r.width - totalWidths;
	spacing = spacing > 0 ? spacing : 0;
	if (stackFrames > 0) {
		int compInterspacing = interspacing * spaces;
		sFrameSpacing = spacing - compInterspacing;
		spacing = compInterspacing;
	}
	spacing = spacing > 0 ? spacing : 0;
	spacing /= spaces;

	for (std::shared_ptr<GameObject> o : children) {
		o->setPosition(Point(r.x, r.y));
		if (o->isHorizontalStackFrame || o->isVerticalStackFrame) {
			o->size.width = sFrameSpacing / stackFrames;
			o->size.height = r.height;
			static_pointer_cast<StackFrame>(o)->align();
		}
		r.x += spacing;
		r.x += (o->size.width >= 0 ? o->size.width : 0);
	}
}

void HorizontalStackFrame::align()
{
	align({ int(position.x), int(position.y), int(size.width), int(size.height) });
}

}