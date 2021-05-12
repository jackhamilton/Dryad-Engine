#pragma once
#include "Point.h"
#include <utility>
//This is a unit vector with origin zero
class Line {
public:
	double x, y;
	Line(double x, double y, double x2, double y2);
	Line(double dx, double dy);
	Line(int dx, int dy) : Line((double)dx, (double)dy) {};
	Line(double r, int theta);
	Line() : Line(0, 0) {};

	friend Line operator+(const Line& a, const Line& b);
	friend Line operator-(const Line& a, const Line& b);
	friend Line operator*(const Line& a, const double b);
	friend Line operator*(const double a, const Line& b);
	friend bool operator>(const Line& a, const Line& b);
	friend bool operator<(const Line& a, const Line& b);

	Line& operator+=(const Line& b);
	Line& operator+=(const double b);
	Line& operator-=(const Line& b);
	Line& operator-=(const double b);
	Line& operator*=(const double b);

	Point getCartesian();
	//r then theta
	std::pair<double, double> getPolar();
};