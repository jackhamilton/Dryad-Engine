#pragma once

#include <algorithm>
#include <GameObject.h>
#include <Point.h>

struct Vector {
	double x;
	double y;
};

struct AABB {
	struct Vector min;
	struct Vector max;
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
	Physics(GameObject* object);

	//Getters and setters
	double getRestitution();
	void setRestitution(double r);
	double getMass();
	void setMass(double m);
	struct Vector getVelocity();
	void setVelocity(struct Vector v);
	Point getPosition();
	AABB getBounds();
	void setBounds(AABB b);

	//Call if the sprite is changed
	void computeBounds();
	
	//If enabled, the physics object will not react to gravity, impulses, or collisions.
	bool positionFixed;
private:
	struct Vector velocity;
	double restitution;
	double mass;
	GameObject* object;
	AABB bounds;
};