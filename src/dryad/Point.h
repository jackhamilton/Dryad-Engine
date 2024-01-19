#pragma once

namespace dryad
{

class Point {
public:
	double x, y;
	Point(double xPosition, double yPosition);
	Point(int xPosition, int yPosition) : Point((double)xPosition, (double)yPosition) {};
	Point() : Point(0, 0) {};

	friend Point operator+(const Point& a, const Point& b);
	friend Point operator-(const Point& a, const Point& b);
	friend Point operator*(const Point& a, double b);
	//Uses weird implementation for efficiency, purely for set, map, and list sorting
	friend bool operator<(const Point& a, const Point& b);
	friend bool operator!=(const Point& a, const Point& b);
	friend bool operator==(const Point& a, const Point& b);

	Point& operator+=(const Point& b);
	Point& operator-=(const Point& b);

	double distance(Point p);
};

}