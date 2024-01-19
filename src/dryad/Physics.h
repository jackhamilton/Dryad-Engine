#pragma once

#include <algorithm>
#include <GameObject.h>
#include <memory>
#include <Point.h>

namespace dryad
{

struct AABB {
	Vector min;
	Vector max;
};

//Handles 2D physics
class Physics {
public:
	//Static functions
	static bool testAABBCollision(AABB a, AABB b);
	static void resolveSimpleCollision(Physics A, Physics B);
	static void resolveComplexCollision(Physics A, Physics B);
	static double dotProduct(Vector A, Vector B);

	//Constructor
	Physics(shared_ptr<GameObject> object);

	//Getters and setters
	double getRestitution();
	void setRestitution(double r);
	double getMass();
	void setMass(double m);
	Vector getVelocity();
	void setVelocity(Vector v);
	Point getPosition();
	AABB getBounds();
	void setBounds(AABB b);

	//Call if the sprite is changed
	void computeBounds();
	
	//If enabled, the physics object will not react to gravity, impulses, or collisions.
	bool positionFixed;
private:
	Vector velocity;
	double restitution;
	double mass;
	shared_ptr<GameObject> object;
	AABB bounds;
};

}