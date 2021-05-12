#include <math.h>
#include "Vector.h"

Vector::Vector(double x, double y, double x2, double y2) {
	Vector::x = x2 - x;
	Vector::y = y2 - y;
}

Vector::Vector(double dx, double dy) {
	Vector::x = dx, Vector::y = dy;
}

Vector::Vector(double r, int theta) {
	x = r * cos(theta);
	y = r * sin(theta);
}

Point Vector::getCartesian()
{
	return Point(x, y);
}

std::pair<double, double> Vector::getPolar()
{
	double r = abs(sqrt(pow(x, 2) + pow(y, 2)));
	double theta = acos(x / r);
	return std::make_pair(r, theta);
}

Vector operator+(const Vector& a, const Vector& b)
{
	return Vector(a.x + b.x, a.y + b.y);
}

Vector operator-(const Vector& a, const Vector& b)
{
	return Vector(a.x - b.x, a.y - b.y);
}

Vector operator*(const Vector& a, const double b)
{
	double r = abs(sqrt(pow(a.x, 2) + pow(a.y, 2)));
	double theta = acos(a.x / r);
	r *= b;
	return Vector(r * cos(theta), r * sin(theta));
}

Vector operator*(const double a, const Vector& b)
{
	double r = abs(sqrt(pow(b.x, 2) + pow(b.y, 2)));
	double theta = acos(b.x / r);
	r *= a;
	return Vector(r * cos(theta), r * sin(theta));
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
	double theta = acos(x / r);
	r += b;
	x = r * cos(theta);
	y = r * sin(theta);
	return *this;
}

Vector& Vector::operator-=(const double b)
{
	double r = abs(sqrt(pow(x, 2) + pow(y, 2)));
	double theta = acos(x / r);
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
	double theta = acos(x / r);
	r *= b;
	x = r * cos(theta);
	y = r * sin(theta);
	return *this;
}