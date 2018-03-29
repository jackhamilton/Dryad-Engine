#pragma once

using namespace std;

class Point {
public:
	double x, y;
	Point(double xPosition, double yPosition);
	Point() : Point(0, 0) {};
};