#pragma once
#include "Point.h"
#include "Window.h"
#include <memory>
#include "GameObject.h"

namespace dryad
{

class Camera
{
public:
	Camera(Resolution* r);
	Resolution* resolution;
	Point getPosition();
	Point getPositionModifier();
	Point getOrigin();
	//Cannot currently track a child object
	void setTrackObject(std::shared_ptr<GameObject> object);
	void stopTracking();
	bool getIsTracking();
private:
	bool tracking = false;
    std::shared_ptr<GameObject> trackObj;
	Point origin;
	friend class World;
};


}
