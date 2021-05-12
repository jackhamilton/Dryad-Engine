#pragma once
#include "Point.h"
#include <utility>
//This is a unit vector with origin zero
class Vector {
public:
	double x, y;
	Vector(double x, double y, double x2, double y2);
	Vector(double dx, double dy);
	Vector(int dx, int dy) : Vector((double)dx, (double)dy) {};
	Vector(double r, int theta);
	Vector() : Vector(0, 0) {};

	friend Vector operator+(const Vector& a, const Vector& b);
	friend Vector operator-(const Vector& a, const Vector& b);
	friend Vector operator*(const Vector& a, const double b);
	friend Vector operator*(const double a, const Vector& b);
	friend bool operator>(const Vector& a, const Vector& b);
	friend bool operator<(const Vector& a, const Vector& b);

	Vector& operator+=(const Vector& b);
	Vector& operator+=(const double b);
	Vector& operator-=(const Vector& b);
	Vector& operator-=(const double b);
	Vector& operator*=(const double b);

	Point getCartesian();
	//r then theta
	std::pair<double, double> getPolar();
};