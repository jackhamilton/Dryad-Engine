#pragma once
#include "Point.h"
#include "Window.h"
#include <memory>
#include "GameObject.h"

class Camera
{
public:
	Camera(Resolution* r);
	Resolution* resolution;
	Point getPosition();
	Point getPositionMod();
	//Cannot currently track a child object
	void setTrackObject(shared_ptr<GameObject> object);
	void stopTracking();
	bool getIsTracking();
private:
	bool tracking = false;
	shared_ptr<GameObject> trackObj;
	Point origin;
	friend class World;
};

