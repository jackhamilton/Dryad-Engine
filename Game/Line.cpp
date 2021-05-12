#include <math.h>
#include "Line.h"

Line::Line(double x, double y, double x2, double y2) {
	Line::x = x2 - x;
	Line::y = y2 - y;
}

Line::Line(double dx, double dy) {
	Line::x = dx, Line::y = dy;
}

Line::Line(double r, int theta) {
	x = r * cos(theta);
	y = r * sin(theta);
}

Point Line::getCartesian()
{
	return Point(x, y);
}

std::pair<double, double> Line::getPolar()
{
	double r = abs(sqrt(pow(x, 2) + pow(y, 2)));
	double theta = acos(x / r);
	return std::make_pair(r, theta);
}

Line operator+(const Line& a, const Line& b)
{
	return Line(a.x + b.x, a.y + b.y);
}

Line operator-(const Line& a, const Line& b)
{
	return Line(a.x - b.x, a.y - b.y);
}

Line operator*(const Line& a, const double b)
{
	double r = abs(sqrt(pow(a.x, 2) + pow(a.y, 2)));
	double theta = acos(a.x / r);
	r *= b;
	return Line(r * cos(theta), r * sin(theta));
}

Line operator*(const double a, const Line& b)
{
	double r = abs(sqrt(pow(b.x, 2) + pow(b.y, 2)));
	double theta = acos(b.x / r);
	r *= a;
	return Line(r * cos(theta), r * sin(theta));
}

bool operator>(const Line& a, const Line& b)
{
	double r = abs(sqrt(pow(a.x, 2) + pow(a.y, 2)));
	double r2 = abs(sqrt(pow(b.x, 2) + pow(b.y, 2)));
	return r > r2;
}

bool operator<(const Line& a, const Line& b)
{
	double r = abs(sqrt(pow(a.x, 2) + pow(a.y, 2)));
	double r2 = abs(sqrt(pow(b.x, 2) + pow(b.y, 2)));
	return r < r2;
}

Line& Line::operator+=(const Line& b)
{
	x += b.x;
	y += b.y;
	return *this;
}

Line& Line::operator+=(const double b)
{
	double r = abs(sqrt(pow(x, 2) + pow(y, 2)));
	double theta = acos(x / r);
	r += b;
	x = r * cos(theta);
	y = r * sin(theta);
	return *this;
}

Line& Line::operator-=(const double b)
{
	double r = abs(sqrt(pow(x, 2) + pow(y, 2)));
	double theta = acos(x / r);
	r -= b;
	x = r * cos(theta);
	y = r * sin(theta);
	return *this;
}

Line& Line::operator-=(const Line& b)
{
	x -= b.x;
	y -= b.y;
	return *this;
}

Line& Line::operator*=(const double b)
{
	double r = abs(sqrt(pow(x, 2) + pow(y, 2)));
	double theta = acos(x / r);
	r *= b;
	x = r * cos(theta);
	y = r * sin(theta);
	return *this;
}