#include "Point.h"
#include <math.h>

Point::Point(double xPos, double yPos) {
	Point::x = xPos, Point::y = yPos;
}
Point& Point::operator+=(const Point& b)
{
	x += b.x;
	y += b.y;
	return *this;
}
Point& Point::operator-=(const Point& b)
{
	x -= b.x;
	y -= b.y;
	return *this;
}

Point operator+(const Point& a, const Point& b)
{
	return Point(a.x + b.x, a.y + b.y);
}

Point operator-(const Point& a, const Point& b)
{
	return Point(a.x - b.x, a.y - b.y);
}

bool operator<(const Point& a, const Point& b)
{
	return a.x + a.y > b.x + b.y;
}

bool operator!=(const Point& a, const Point& b)
{
	return a.x != b.x || a.y != b.y;
}

bool operator==(const Point& a, const Point& b)
{
	return a.x == b.x && a.y == b.y;
}

double Point::distance(Point p)
{
	return sqrt(pow(p.x - x, 2) + pow(p.y - y, 2) * 1.0);
}
