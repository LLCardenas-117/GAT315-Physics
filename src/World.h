#pragma once

//#include "Body.h"
#include "Collision.h"

#include <vector>

class World {
public:
	static Vector2 gravity;
	std::vector<Body> bodies;
	std::vector<class Effector*> effectors;
	std::vector<Contact> contacts;

	Body* GetBodyIntersect(Vector2 position);
	static void SetGravity(Vector2 newgravity) { gravity = newgravity; }

	void SetBounds(Vector2 min, Vector2 max) { boundsMin = min; boundsMax = max; }

public:
	World();
	void Step(float deltaTime);
	void Draw();
	void AddBody(const Body& body);
	void AddEffector(Effector* effector);
	void UpdateCollision();

private:
	Vector2 boundsMin{ -10.0f, -5.0f };
	Vector2 boundsMax{ 10.0f, 5.0f };
};