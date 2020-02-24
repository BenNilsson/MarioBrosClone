#include "Collisions.h"
#include "Character.h"

// Initialise the instance to null
Collisions* Collisions::mInstance = NULL;

Collisions::Collisions()
{

}

Collisions::~Collisions()
{
	mInstance = NULL;
}

Collisions* Collisions::Instance()
{
	if (!mInstance)
		mInstance = new Collisions;

	return mInstance;
}

bool Collisions::Circle(Circle2D circle1, Circle2D circle2)
{
	// Calculate the vector that seperates the two objects
	Vector2D vec = Vector2D((circle1.position.x - circle2.position.x), (circle1.position.y - circle2.position.y));

	// Calculate the length of the vector
	double distance = sqrt((vec.x * vec.x) + (vec.y * vec.y));

	// Get collision radius of each character and accumulate them
	double combinedDistance = ((double)circle1.circleRadius + (double)circle2.circleRadius);

	// heck whether the distance is shorter than the accumulated collision radii
	return distance < combinedDistance;
}

bool Collisions::Box(Rect2D rect1, Rect2D rect2)
{
	if (rect1.x < rect2.x + rect2.width &&
		rect1.x + rect1.width > rect2.x &&
		rect1.y < rect2.y + rect2.height &&
		rect1.y + rect1.height > rect2.y)
	{
		return true;
	}

	return false;
}
