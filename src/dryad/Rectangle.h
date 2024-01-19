#pragma once
#include "Point.h"

namespace dryad
{

struct Rectangle {
	int x;
	int y;
	int width;
	int height;
	bool isInside(Point p) {
		if (p.x > x and p.x < x + width and p.y > y and p.y < y + height) {
			return true;
		}
		return false;
	}
};

}