#include "Camera.h"

namespace dryad
{

Camera::Camera(Resolution* r)
{
	resolution = r;
}

//gets camera center
Point Camera::getPosition()
{
	if (tracking && trackObj) {
		return trackObj->getPosition();
	}
	tracking = false;
	return origin;
}

Point Camera::getPositionModifier() {
	if (!trackObj || !tracking) {
		return Point(0, 0);
	}
	std::pair<int, int> cameraRes = Window::calculateResolution(*resolution);
	return Point(cameraRes.first / 2, cameraRes.second / 2)
		- getPosition()
		- Point(trackObj->getSize().width / 2, trackObj->getSize().height / 2);
}

Point Camera::getOrigin() {
	if (!trackObj || !tracking) {
		return origin;
	}
	std::pair<int, int> cameraRes = Window::calculateResolution(*resolution);
	return trackObj->getPosition() 
		- (Point(cameraRes.first / 2, cameraRes.second / 2)
		- getPosition()
		- Point(trackObj->getSize().width/2, trackObj->getSize().height/2));
}

void Camera::setTrackObject(std::shared_ptr<GameObject> object)
{
	tracking = true;
	trackObj = object;
}

void Camera::stopTracking()
{
	tracking = false;
}

bool Camera::getIsTracking()
{
	return tracking;
}

}
