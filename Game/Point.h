#pragma once

using namespace std;

class Point {
public:
	int x, y;
	Point(int xPosition, int yPosition);
	Point() : Point(0, 0) {};
};