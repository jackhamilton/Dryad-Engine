#pragma once
#include "Rectangle.h"
#include "Circle.h"
#include "Vector.h"
#include <vector>

using namespace std;
class GameObject;

class Hitbox
{
public:
	Hitbox(Circle c);
	Hitbox(Rectangle r);
	//UNIMPLEMENTED
	//does not work with rotated rectangles
	bool testCollision(Hitbox h);
	Point getCenter();
	Vector getMaximumClearDistanceForVectorFromGameObject(std::vector<Hitbox*> objects, Vector vector);
	//Rotates point around center of hitbox
	static Point rotate(Point p, double deg);
	vector<Point> getCorners();
private:
	int rectCollisionTestPrecision = 3;
	double rotation;
	bool usesCircleHitbox;
	Circle hitboxCircle;
	Rectangle hitboxRect;
	vector<Point> getRectangularPointsSet();

	static vector<Point> getEquallySpacedPointsAlongLine(Point origin, Vector a, int pts);
	static bool lineCircle(double x1, double y1, double x2, double y2, double cx, double cy, double r, Point* nearestCollision);
	static bool lineRect(double x1, double y1, double x2, double y2, double rx, double ry, double rx2, double ry2, Point* nearestCollision);
	static bool lineLine(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, Point* collision);
	static bool linePoint(double x1, double y1, double x2, double y2, double px, double py);
	static bool pointCircle(double px, double py, double cx, double cy, double r);
	//UNIMPLEMENTED FULLY
	static bool testCircleCollision(Hitbox* h1, Hitbox* h2);
	//circle first, does not work with rotation
	static bool testHybridCollision(Hitbox* h1, Hitbox* h2);
	//does not work with rotation
	static bool testFixedRectangleCollision(Hitbox* h1, Hitbox* h2);
};

