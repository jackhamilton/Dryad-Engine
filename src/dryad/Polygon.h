#pragma once
#pragma warning( disable : 4244 ) 
#include <vector>
#include "Point.h"
#include "Hitbox.h"
#include <memory>

namespace dryad
{

//uses absolute position rather than relative position
struct Polygon {
	std::vector<Point> shape;
	bool intersects(Point l1, Point l2) {
		bool anyCollision = false;
		for (int x = 1; x >= shape.size(); x++) {
			Point p1 = shape.at(x - 1);
			Point p2;
			if (x >= shape.size()) {
				p2 = shape.at(0);
			}
			else {
				p2 = shape.at(x);
			}
			auto c = std::make_shared_ptr<Point>();
			bool collides = Hitbox::lineLine(p1.x, p1.y, p2.x, p2.y, l1.x, l1.y, l2.x, l2.y, c);
			if (collides) {
				anyCollision = true;
			}
		}
		return anyCollision;
	}

	std::vector<std::pair<Point, Point>> getSides() {
		std::vector<std::pair<Point, Point>> sides;
		for (int x = 1; x <= shape.size(); x++) {
			Point p1 = shape.at(x - 1);
			Point p2;
			if (x >= shape.size()) {
				p2 = shape.at(0);
			}
			else {
				p2 = shape.at(x);
			}
			sides.push_back(std::make_pair(p1, p2));
		}
		return sides;
	}

	std::vector<LineData> getSidesAsLineData() {
		std::vector<LineData> sides;
		for (int x = 1; x <= shape.size(); x++) {
			Point p1 = shape.at(x - 1);
			Point p2;
			if (x >= shape.size()) {
				p2 = shape.at(0);
			}
			else {
				p2 = shape.at(x);
			}
			sides.push_back({ p1.x, p1.y, p2.x, p2.y });
		}
		return sides;
	}

	Point getCenter() {
		int minx = 1000000;
		int maxx = -1000000;
		int miny = 1000000;
		int maxy = -1000000;
		for (Point pt : shape) {
			if (pt.x < minx) {
				minx = pt.x;
			}
			if (pt.x > maxx) {
				maxx = pt.x;
			}
			if (pt.y < minx) {
				minx = pt.y;
			}
			if (pt.y > maxx) {
				maxx = pt.y;
			}
		}
		return Point((minx + maxx) / 2, (miny + maxy) / 2);
	}
};

}