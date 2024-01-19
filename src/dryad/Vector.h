#pragma once
#include "Point.h"
#include <utility>
//This is a unit vector with origin zero
#define PI 3.14159265

namespace dryad
{

class Vector {
public:
	double x, y;
	Vector(double x, double y, double x2, double y2);
	Vector(Point p);
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

class PolarVector {
public:
	//theta stored as radians
	double r, theta;
	//degrees
	PolarVector(double x1, double y1, double x2, double y2) {
		r = abs(sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2)));
		theta = atan2(y2 - y1, x2 - x1);
	}
	PolarVector(double r, double theta) {
		double rad = (((double)theta) * PI) / 180.0;
		PolarVector::r = r;
		PolarVector::theta = rad;
	};
	PolarVector() : PolarVector(0.0, 0.0) {};

	//degrees
	void setTheta(double theta) {
		if (theta >= 360) {
			theta -= 360;
		}
		PolarVector::theta = (theta * PI) / 180.0;
	}

	void setThetaRadians(double theta) {
		PolarVector::theta = theta;
	}

	double getThetaDegrees() {
		return theta * 180 / PI;
	}

	Point getCartesian() {
		return Point(r * cos(theta), r * sin(theta));
	}

	friend PolarVector operator+(const PolarVector& a, const PolarVector& b);
	friend PolarVector operator-(const PolarVector& a, const PolarVector& b);
	friend PolarVector operator+(const PolarVector& a, const double b);
	friend PolarVector operator-(const PolarVector& a, const double b);

	PolarVector& operator+=(const double b);
	PolarVector& operator-=(const double b);
};

struct LineData {
	double x1, y1, x2, y2;

	double distToPoint(Point point) {
		return abs((x2 - x1) * (y1 - point.y) - (x1 - point.x) * (y2 - y1)) / sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
	}
};

}