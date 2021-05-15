#include "Hitbox.h"
#include <stdio.h>
#include <vector>
#include "Point.h"

Hitbox::Hitbox(Circle c, Point* parentPosition)
{
	hitboxCircle = c;
	usesCircleHitbox = true;
	Hitbox::parentPosition = parentPosition;
}

Hitbox::Hitbox(Rectangle r, Point* parentPosition)
{
	hitboxRect = r;
	usesCircleHitbox = false;
	rotation = 0;
	Hitbox::parentPosition = parentPosition;
}

bool Hitbox::testCollision(Hitbox h)
{
	//unfinished
	return true;
	if (h.usesCircleHitbox) {
		if (usesCircleHitbox) {
			return testCircleCollision(this, &h);
		}
		else {
			return testHybridCollision(&h, this);
		}
	}
	else {
		if (usesCircleHitbox) {
			return testHybridCollision(this, &h);
		}
		else {
			return testFixedRectangleCollision(this, &h);
		}
	}
}

Point Hitbox::getCenter()
{
	if (usesCircleHitbox) {
		return hitboxCircle.center;
	}
	else {
		return Point(hitboxRect.x + hitboxRect.width / 2, hitboxRect.y + hitboxRect.height / 2);
	}
}

//Assumes vector calculated from object origin. Hitbox array is for all other gameobjects
std::pair<Vector, Collision> Hitbox::getMaximumClearDistanceForVectorFromGameObject(vector<Hitbox*> objects, Vector vector)
{
	bool clear = true;
	Point colPoint;
	Vector minimumLineGlobal(Point(650000, 650000));
	for (Hitbox* o : objects) {
		Vector vecCopy = Vector(Point(vector.x, vector.y));
		Point myCenter = getCenter() + *parentPosition;
		double x = myCenter.x;
		double y = myCenter.y;
		Hitbox* targetHitbox = o;
		Point center = targetHitbox->getCenter() + *targetHitbox->parentPosition;
		bool objectClear = true;
		if (usesCircleHitbox and targetHitbox->usesCircleHitbox) {
			//both circles
			vecCopy += hitboxCircle.r;
			Point nearestCollision;
			objectClear = lineCircle(x, y, x + vecCopy.x, y + vecCopy.y, center.x, center.y, targetHitbox->hitboxCircle.r, &nearestCollision);
			if (!objectClear) {
				Vector ret = Vector(x, y, nearestCollision.x, nearestCollision.y);
				ret -= (double)(hitboxCircle.r);
				clear = false;
				if (ret < minimumLineGlobal) {
					minimumLineGlobal = ret;
				}
			}
		} else if (!targetHitbox->usesCircleHitbox && !usesCircleHitbox) {
			//both rectangles
			std::vector<Point> origins = getRectangularPointsSet();
			Vector minimumLine(Point(650000, 650000));
			bool collisionRegistered = false;
			for (Point p1 : origins) {
				Point pt = p1 + *parentPosition;
				std::vector<Point> targetCorners = targetHitbox->getCorners();
				Point c1 = targetCorners.at(0) + *targetHitbox->parentPosition;
				Point c2 = targetCorners.at(2) + *targetHitbox->parentPosition;
				Point collision;
				bool collided = lineRect(pt.x, pt.y, pt.x + vecCopy.x, pt.y + vecCopy.y, c1.x, c1.y, c2.x, c2.y, &collision);
				if (collided) {
					collisionRegistered = true;
					Vector collVector = Vector(pt.x, pt.y, collision.x, collision.y);
					if (collVector < minimumLine) {
						minimumLine = collVector;
					}
				}
			}
			if (collisionRegistered) {
				clear = false;
			}
			else {
				if (minimumLine < minimumLineGlobal) {
					minimumLineGlobal = minimumLine;
				}
			}
		}
		else {
			//hybrid
			if (targetHitbox->usesCircleHitbox) {
				//target circle, source rect
				std::vector<Point> origins = getRectangularPointsSet();
				Vector minimumLine(Point(650000, 650000));
				bool collisionRegistered = false;
				for (Point p1 : origins) {
					Point pt = p1 + *parentPosition;
					Point collision;
					Circle c1 = targetHitbox->hitboxCircle;
					bool collided = lineCircle(pt.x, pt.y, pt.x + vecCopy.x, pt.y + vecCopy.y, center.x, center.y, c1.r, &collision);
					if (collided) {
						collisionRegistered = true;
						Vector collVector = Vector(pt.x, pt.y, collision.x, collision.y);
						if (collVector < minimumLine) {
							minimumLine = collVector;
						}
					}
				}
				if (collisionRegistered) {
					clear = false;
				}
				else {
					if (minimumLine < minimumLineGlobal) {
						minimumLineGlobal = minimumLine;
					}
				}
			}
			else {
				//target rect, source circle
				vecCopy += hitboxCircle.r;
				Point nearestCollision;
				std::vector<Point> targetCorners = targetHitbox->getCorners();
				Point c1 = targetCorners.at(0) + *targetHitbox->parentPosition;
				Point c2 = targetCorners.at(2) + *targetHitbox->parentPosition;
				objectClear = lineRect(myCenter.x, myCenter.x, myCenter.x + vecCopy.x, myCenter.y + vecCopy.y, c1.x, c1.y, c2.x, c2.y, &nearestCollision);
				if (!objectClear) {
					Vector ret = Vector(x, y, nearestCollision.x, nearestCollision.y);
					ret -= (double)(hitboxCircle.r);
					clear = false;
					if (ret < minimumLineGlobal) {
						minimumLineGlobal = ret;
					}
				}
			}
		}
	}
	if (clear) {
		Collision c = { false, colPoint };
		return make_pair(vector, c);
	}
	else if (minimumLineGlobal.x < 640000 && minimumLineGlobal.y < 640000) {
		Collision c = { true, colPoint };
		return make_pair(minimumLineGlobal, c);
	}
	Collision c = { true, colPoint };
	return make_pair(Vector(Point(0, 0)), c);
}

//relative to parent position
vector<Point> Hitbox::getRectangularPointsSet() {
	vector<Point> points;
	if (!usesCircleHitbox) {
		Point cCorner = rotate(Point(hitboxRect.x, hitboxRect.y), rotation);
		Vector line = Vector(hitboxRect.width, (int)rotation );
		bool first = true;
		for (Point p : getEquallySpacedPointsAlongLine(cCorner, line, rectCollisionTestPrecision)) {
			if (!first) {
				points.push_back(p);
			}
			else {
				first = false;
			}
		}
		first = true;
		cCorner = Point(cCorner.x + line.x, cCorner.y + line.y);
		line = Vector(hitboxRect.height, (int)(90 + rotation));
		for (Point p : getEquallySpacedPointsAlongLine(cCorner, line, rectCollisionTestPrecision)) {
			if (!first) {
				points.push_back(p);
			}
			else {
				first = false;
			}
		}
		first = true;
		cCorner = Point(cCorner.x + line.x, cCorner.y + line.y);
		line = Vector(hitboxRect.width, (int)(180 + rotation));
		for (Point p : getEquallySpacedPointsAlongLine(cCorner, line, rectCollisionTestPrecision)) {
			if (!first) {
				points.push_back(p);
			}
			else {
				first = false;
			}
		}
		first = true;
		cCorner = Point(cCorner.x + line.x, cCorner.y + line.y);
		line = Vector(hitboxRect.height, (int)(270 + rotation));
		for (Point p : getEquallySpacedPointsAlongLine(cCorner, line, rectCollisionTestPrecision)) {
			if (!first) {
				points.push_back(p);
			}
			else {
				first = false;
			}
		}
	}
	return points;
}

//relative to parent position
vector<Point> Hitbox::getCorners() {
	vector<Point> points;
	if (!usesCircleHitbox) {
		Point cCorner = rotate(Point(hitboxRect.x, hitboxRect.y), rotation);
		points.push_back(cCorner);
		Vector line = Vector(hitboxRect.width, { (int)rotation });
		cCorner = Point(cCorner.x + line.x, cCorner.y + line.y);
		points.push_back(cCorner);
		line = Vector(hitboxRect.height, { (int)(90 + rotation) });
		cCorner = Point(cCorner.x + line.x, cCorner.y + line.y);
		points.push_back(cCorner);
		line = Vector(hitboxRect.width, { (int)(180 + rotation) });
		cCorner = Point(cCorner.x + line.x, cCorner.y + line.y);
		points.push_back(cCorner);
	}
	return points;
}

vector<Point> Hitbox::getEquallySpacedPointsAlongLine(Point origin, Vector a, int pts) {
	vector<Point> points;
	double length = a.getPolar().first;
	double spacing = length / (pts - 1);
	for (int i = pts; i > 0; i--) {
		points.push_back(Point(origin.x + a.x, origin.y + a.y));
		if (i > 1) {
			a -= spacing;
		}
	}
	return points;
}

bool Hitbox::testCircleCollision(Hitbox* h1, Hitbox* h2)
{
	Circle c1 = h1->hitboxCircle;
	Circle c2 = h2->hitboxCircle;
	return false;
}

//circle first
bool Hitbox::testHybridCollision(Hitbox* h1, Hitbox* h2)
{
	Circle c = h1->hitboxCircle;
	Rectangle r = h2->hitboxRect;
	return false;
}

bool Hitbox::testFixedRectangleCollision(Hitbox* h1, Hitbox* h2)
{
	Rectangle r1 = h1->hitboxRect;
	Rectangle r2 = h2->hitboxRect;
	if (r1.x + r1.width < r2.x || r1.x > r2.x + r2.width) return false;
	if (r1.y + r1.height < r2.y || r1.y > r2.y + r2.height) return false;
	return true;
}

bool Hitbox::lineRect(double x1, double y1, double x2, double y2, double rx, double ry, double rx2, double ry2, Point* nearestCollision) {

	// check if the line has hit any of the rectangle's sides
	// uses the Line/Line function below
	Point collision = Point(-650000, -650000);
	bool left = lineLine(x1, y1, x2, y2, rx, ry, rx, ry2, &collision);
	Point collision2 = Point(-650000, -650000);
	bool right = lineLine(x1, y1, x2, y2, rx2, ry, rx2, ry2, &collision2);
	Point collision3 = Point(-650000, -650000);
	bool top = lineLine(x1, y1, x2, y2, rx, ry, rx2, ry, &collision3);
	Point collision4 = Point(-650000, -650000);
	bool bottom = lineLine(x1, y1, x2, y2, rx, ry2, rx2, ry2, &collision4);
	Point points [4] = { collision, collision2, collision3, collision4 };
	Point nearestCollisionLocal = Point(650000, 650000);
	double nearestCollisionDist = 650000;
	for (Point p : points) {
		if (p.x != -650000 and p.y != -650000) {
			if (p.distance(Point(x1, y1)) < nearestCollisionDist) {
				nearestCollisionLocal.x = x1;
				nearestCollisionLocal.y = y1;
				nearestCollisionDist = p.distance(Point(x1, y1));
			}
		}
	}

	// if ANY of the above are true, the line
	// has hit the rectangle
	if (left || right || top || bottom) {
		if (nearestCollisionDist == 650000) {
			printf("Error: unknown failure in line-rect hitbox collision calculation.\n");
		}
		nearestCollision->x = nearestCollisionLocal.x;
		nearestCollision->y = nearestCollisionLocal.y;
		return true;
	}
	return false;
}

bool Hitbox::lineLine(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, Point* collision) {

	// calculate the direction of the lines
	double uA = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
	double uB = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));

	// if uA and uB are between 0-1, lines are colliding
	if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1) {

		// optionally, draw a circle where the lines meet
		double intersectionX = x1 + (uA * (x2 - x1));
		double intersectionY = y1 + (uA * (y2 - y1));
		collision->x = intersectionX;
		collision->y = intersectionY;
		return true;
	}
	
	return false;
}

// LINE/CIRCLE
bool Hitbox::lineCircle(double x1, double y1, double x2, double y2, double cx, double cy, double r, Point* nearestCollision) {
	/*
	// is either end INSIDE the circle?
	// if so, return true immediately
	bool inside1 = pointCircle(x1, y1, cx, cy, r);
	bool inside2 = pointCircle(x2, y2, cx, cy, r);
	if (inside1 || inside2) return true;*/

	// get length of the line
	double distX = x1 - x2;
	double distY = y1 - y2;
	double len = sqrt((distX * distX) + (distY * distY));

	// get dot product of the line and circle
	double dot = (((cx - x1) * (x2 - x1)) + ((cy - y1) * (y2 - y1))) / pow(len, 2);

	// find the closest point on the line
	double closestX = x1 + (dot * (x2 - x1));
	double closestY = y1 + (dot * (y2 - y1));

	// is this point actually on the line segment?
	// if so keep going, but if not, return false
	bool onSegment = linePoint(x1, y1, x2, y2, closestX, closestY);
	if (!onSegment) return false;

	// get distance to closest point
	distX = closestX - cx;
	distY = closestY - cy;
	double distance = sqrt((distX * distX) + (distY * distY));

	if (distance <= r) {
		nearestCollision->x = closestX;
		nearestCollision->y = closestY;
		return true;
	}
	return false;
}

Point Hitbox::rotate(Point p, double deg) {

	double s = sin(deg); // angle is in radians
	double c = cos(deg); // angle is in radians

	double xnew = p.x * c + p.y * s;
	double ynew = -p.x * s + p.y * c;
	return Point(xnew, ynew);
}

// POINT/CIRCLE
bool Hitbox::pointCircle(double px, double py, double cx, double cy, double r) {

	// get distance between the point and circle's center
	// using the Pythagorean Theorem
	double distX = px - cx;
	double distY = py - cy;
	double distance = sqrt((distX * distX) + (distY * distY));

	// if the distance is less than the circle's
	// radius the point is inside!
	if (distance <= r) {
		return true;
	}
	return false;
}


// LINE/POINT
bool Hitbox::linePoint(double x1, double y1, double x2, double y2, double px, double py) {

	// get distance from the point to the two ends of the line
	double d1 = Point(px, py).distance(Point(x1, y1));
	double d2 = Point(px, py).distance(Point(x2, y2));

	// get the length of the line
	double lineLen = Point(x1, y1).distance(Point(x2, y2));

	// since floats are so minutely accurate, add
	// a little buffer zone that will give collision
	double buffer = 0.1;    // higher # = less accurate

	// if the two distances are equal to the line's
	// length, the point is on the line!
	// note we use the buffer here to give a range,
	// rather than one #
	if (d1 + d2 >= lineLen - buffer && d1 + d2 <= lineLen + buffer) {
		return true;
	}
	return false;
}