#pragma once
#include "Rectangle.h"
#include "Circle.h"
#include "Vector.h"
#include <memory>
#include <string>
#include <vector>

using namespace std;
class GameObject;

struct Collision {
	bool collided;
	Point p;
};

class Hitbox
{
public:
	Hitbox(Circle c, Point* parentPosition);
	Hitbox(Rectangle r, Point* parentPosition);
	//UNIMPLEMENTED
	//does not work with rotated rectangles
	bool testCollision(Hitbox h);
	Point getCenter();
	//second return is whether a collision happened
	std::pair<Vector, Collision> getMaximumClearDistanceForVectorFromGameObject(std::vector<shared_ptr<Hitbox>> objects, Vector vector);
	//Rotates point around center of hitbox
	static Point rotate(Point p, double deg);
	vector<Point> getCorners();
private:
	Point* parentPosition;
	int rectCollisionTestPrecision = 3;
	double rotation;
	bool usesCircleHitbox;
	Circle hitboxCircle;
	Rectangle hitboxRect;
	vector<Point> getRectangularPointsSet();

	static vector<Point> getEquallySpacedPointsAlongLine(Point origin, Vector a, int pts);
	static bool lineCircle(double x1, double y1, double x2, double y2, double cx, double cy, double r, shared_ptr<Point> nearestCollision);
	static bool lineRect(double x1, double y1, double x2, double y2, double rx, double ry, double rx2, double ry2, shared_ptr<Point> nearestCollision);
	static bool lineLine(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, shared_ptr<Point> collision);
	static bool linePoint(double x1, double y1, double x2, double y2, double px, double py);
	static bool pointCircle(double px, double py, double cx, double cy, double r);
	//UNIMPLEMENTED FULLY
	static bool testCircleCollision(shared_ptr<Hitbox> h1, shared_ptr<Hitbox> h2);
	//circle first, does not work with rotation
	static bool testHybridCollision(shared_ptr<Hitbox> h1, shared_ptr<Hitbox> h2);
	//does not work with rotation
	static bool testFixedRectangleCollision(shared_ptr<Hitbox> h1, shared_ptr<Hitbox> h2);
};

