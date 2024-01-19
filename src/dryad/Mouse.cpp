#include "Mouse.h"

namespace dryad
{

Mouse::Mouse()
{
}

void Mouse::addMouseMovementEvent(std::function<void(Point)> event) {
	mouseMovementEvents.push_back(event);
}

void Mouse::addMouseClickEvent(std::function<void(Point)> event) {
	mouseClickEvents.push_back(event);
}

void Mouse::addMouseClickUpEvent(std::function<void(Point, Point)> event) {
	mouseClickUpEvents.push_back(event);
}

}
