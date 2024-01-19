#include <math.h>
#include "Vector.h"

namespace dryad
{

Vector::Vector(double x, double y, double x2, double y2) {
	Vector::x = x2 - x;
	Vector::y = y2 - y;
}

Vector::Vector(Point p) {
	Vector::x = p.x, Vector::y = p.y;
}

Vector::Vector(double r, int theta) {
	double rad = (((double)theta) * PI) / 180.0;
	x = r * cos(rad);
	y = r * sin(rad);
}

Point Vector::getCartesian()
{
	return Point(x, y);
}

std::pair<double, double> Vector::getPolar()
{
	double r = abs(sqrt(pow(x, 2) + pow(y, 2)));
	double theta = atan2(y, x);
	return std::make_pair(r, theta);
}

Vector operator+(const Vector& a, const Vector& b)
{
	return Vector(Point(a.x + b.x, a.y + b.y));
}

Vector operator-(const Vector& a, const Vector& b)
{
	return Vector(Point(a.x - b.x, a.y - b.y));
}

Vector operator*(const Vector& a, const double b)
{
	double r = abs(sqrt(pow(a.x, 2) + pow(a.y, 2)));
	double theta = atan2(a.y, a.x);
	r *= b;
	return Vector(Point(r * cos((((double)theta) * PI) / 180.0), r * sin((((double)theta) * PI) / 180.0)));
}

Vector operator*(const double a, const Vector& b)
{
	double r = abs(sqrt(pow(b.x, 2) + pow(b.y, 2)));
	double theta = atan2(b.y, b.x);
	r *= a;
	return Vector(Point(r * cos((((double)theta) * PI) / 180.0), r * sin((((double)theta) * PI) / 180.0)));
}

bool operator>(const Vector& a, const Vector& b)
{
	double r = abs(sqrt(pow(a.x, 2) + pow(a.y, 2)));
	double r2 = abs(sqrt(pow(b.x, 2) + pow(b.y, 2)));
	return r > r2;
}

bool operator<(const Vector& a, const Vector& b)
{
	double r = abs(sqrt(pow(a.x, 2) + pow(a.y, 2)));
	double r2 = abs(sqrt(pow(b.x, 2) + pow(b.y, 2)));
	return r < r2;
}

Vector& Vector::operator+=(const Vector& b)
{
	x += b.x;
	y += b.y;
	return *this;
}

Vector& Vector::operator+=(const double b)
{
	double r = abs(sqrt(pow(x, 2) + pow(y, 2)));
	double theta = atan2(y, x);
	r += b;
	x = r * cos(theta);
	y = r * sin(theta);
	return *this;
}

Vector& Vector::operator-=(const double b)
{
	double r = abs(sqrt(pow(x, 2) + pow(y, 2)));
	double theta = atan2(y, x);
	r -= b;
	x = r * cos(theta);
	y = r * sin(theta);
	return *this;
}

Vector& Vector::operator-=(const Vector& b)
{
	x -= b.x;
	y -= b.y;
	return *this;
}

Vector& Vector::operator*=(const double b)
{
	double r = abs(sqrt(pow(x, 2) + pow(y, 2)));
	double theta = atan2(y, x);
	r *= b;
	x = r * cos(theta);
	y = r * sin(theta);
	return *this;
}

PolarVector operator+(const PolarVector& a, const PolarVector& b)
{
	return PolarVector(a.r + b.r, a.theta + b.theta);
}

PolarVector operator-(const PolarVector& a, const PolarVector& b)
{
	return PolarVector(a.r - b.r, a.theta - b.theta);
}

PolarVector operator+(const PolarVector& a, const double b)
{
	return PolarVector(a.r + b, a.theta);
}

PolarVector operator-(const PolarVector& a, const double b)
{
	return PolarVector(a.r - b, a.theta);
}

PolarVector& PolarVector::operator+=(const double b)
{
	r += b;
	return *this;
}

PolarVector& PolarVector::operator-=(const double b)
{
	r += b;
	return *this;
}

}
