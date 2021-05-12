#include "Point.h"
#include <math.h>

Point::Point(double xPos, double yPos) {
	Point::x = xPos, Point::y = yPos;
}
double Point::distance(Point p)
{
	return sqrt(pow(p.x - x, 2) + pow(p.y - y, 2) * 1.0);
}
;
