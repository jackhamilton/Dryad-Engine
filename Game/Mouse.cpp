#include "Mouse.h"

void Mouse::addMouseMovementEvent(function<void(Point)> event) {
	mouseMovementEvents.push_back(event);
}

void Mouse::addMouseClickEvent(function<void(Point)> event) {
	mouseClickEvents.push_back(event);
}

void Mouse::addMouseClickUpEvent(function<void(Point, Point)> event) {
	mouseClickUpEvents.push_back(event);
}