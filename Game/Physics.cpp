#include <Physics.h>
#include <math.h>
#include <Sprite.h>


bool Physics::testAABBCollision(AABB a, AABB b)
{
	// Exit with no intersection if found separated along an axis
	if (a.max.x < b.min.x || a.min.x > b.max.x) return false;
	if (a.max.y < b.min.y || a.min.y > b.max.y) return false;

	// No separating axis found, therefor there is at least one overlapping axis
	return true;
}

//Do position corrections on collisions between two objects, one of which is fixed.
void Physics::resolveSimpleCollision(Physics A, Physics B) {
	if (A.positionFixed || B.positionFixed && !(A.positionFixed && B.positionFixed)) {
		Physics* fixed = &B;
		Physics* unfixed = &A;
		if (A.positionFixed) {
			fixed = &A;
			unfixed = &B;
		}
		double xOverlap = 0;
		double yOverlap = 0;
		AABB fBounds = fixed->getBounds();
		AABB uBounds = unfixed->getBounds();
		if (fBounds.max.x > uBounds.min.x) {
			xOverlap = uBounds.min.x - fBounds.max.x;
		}
		else if (fBounds.min.x > uBounds.max.x) {
			xOverlap = uBounds.max.x - fBounds.min.x;
		}
		if (fBounds.min.y < uBounds.max.y) {
			yOverlap = uBounds.max.y - fBounds.min.y;
		}
		else if (fBounds.max.y > uBounds.min.y) {
			yOverlap = uBounds.min.y - fBounds.max.y;
		}

		//correct collision
		unfixed->object->setPosition({
			unfixed->object->getPosition().x + (xOverlap * -1),
			unfixed->object->getPosition().y + (yOverlap * -1)});
		//adjust velocity, just give it half velocity in the opposite direction
		if (yOverlap != 0) {
			unfixed->setVelocity({
				unfixed->getVelocity().x,
				(unfixed->getVelocity().y * -1) / 2
				});
		}
		if (xOverlap != 0) {
			unfixed->setVelocity({
				(unfixed->getVelocity().x * -1) / 2,
				unfixed->getVelocity().y
				});
		}
		//possible undeleted pointers, not sure if I need to do that (fixed, unfixed)
	}
}

//Does impulse calculations. Good for bouncing things off each other. 
//Not usually necessary for fighting games. Untested.
void Physics::resolveComplexCollision(Physics A, Physics B)
{
	// Calculate relative velocity
	static Vector rv = { B.getVelocity().x - A.getVelocity().x,
		B.getVelocity().y - A.getVelocity().y };

	//Calculate displacement
	static Vector d = { B.getPosition().x - A.getPosition().x,
		B.getPosition().y - A.getPosition().y };

	// Calculate relative velocity in terms of the normal direction
	double relativeVelocity = dotProduct(rv, d);

	// Do not resolve if velocities are separating
	if (relativeVelocity > 0)
		return;

	// Calculate restitution
	double e = std::min(A.getRestitution(), B.getRestitution());

	// Calculate impulse scalar
	double j = -(1 + e) * relativeVelocity;
	j /= 1 / A.getMass() + 1 / B.getMass();

	// Apply impulse
	struct Vector impulse = {
		j * d.x,
		j * d.y
	};
	//D here replaces the normal, this might not work.
	//Look up collision reflection in 2D. 
	A.setVelocity({ A.getVelocity().x - (1 / A.getMass() * impulse.x),
		A.getVelocity().y - (1 / A.getMass() * impulse.y) });
	B.setVelocity({ B.getVelocity().x + (1 / B.getMass() * impulse.x),
		B.getVelocity().y + (1 / B.getMass() * impulse.y) });
}

//Only works on 2D vectors (Vector struct is two-dimensional)
double Physics::dotProduct(Vector A, Vector B)
{
	double product = 0;
	product += A.x * B.x;
	product += A.y * B.y;
	return product;
}

Physics::Physics(GameObject* object) {
	velocity = { 0, 0 };
	positionFixed = false;
	restitution = 1;
	mass = 1;
	Physics::object = object;
	Point pos = object->getPosition();
	std::pair<int, int> dim = object->getSprite()->getDimensions();
	bounds = {
		{pos.x, pos.y},
		{dim.first + pos.x, dim.second + pos.y}
	};
}

struct Vector Physics::getVelocity()
{
	return velocity;
}

void Physics::setVelocity(Vector v)
{
	velocity = v;
}

Point Physics::getPosition()
{
	return object->getPosition();
}

AABB Physics::getBounds()
{
	return bounds;
}

void Physics::setBounds(AABB b)
{
	bounds = b;
}

void Physics::computeBounds()
{
	Point pos = object->getPosition();
	std::pair<int, int> dim = object->getSprite()->getDimensions();
	bounds = {
		{ pos.x, pos.y },
	{ dim.first + pos.x, dim.second + pos.y }
	};
}

double Physics::getRestitution()
{
	return restitution;
}

void Physics::setRestitution(double r)
{
	restitution = r;
}

double Physics::getMass()
{
	return mass;
}

void Physics::setMass(double m)
{
	mass = m;
}
