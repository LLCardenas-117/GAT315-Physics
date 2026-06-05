#pragma once

//#include "Body.h"
#include "Collision.h"

#include <vector>

class World {
public:
	static Vector2 gravity;
	static float springMultiplier;

	std::vector<Body> bodies;
	std::vector<Contact> contacts;
	std::vector<class Effector*> effectors;
	std::vector<class Spring*> springs;

	Body* GetBodyIntersect(Vector2 position);
	static void SetGravity(Vector2 newgravity) { gravity = newgravity; }
	static void SetSpringMultiplier(float newSpringMultiplier) { springMultiplier = newSpringMultiplier; }

	void SetBounds(Vector2 min, Vector2 max) { boundsMin = min; boundsMax = max; }

public:
	World();
	void Step(float deltaTime);
	void Draw();
	void AddBody(const Body& body);
	void AddEffector(Effector* effector);
	void AddSpring(Body& bodyA, Body& bodyB, float restLength, float stiffness, float damping);
	void UpdateCollision();

private:
	Vector2 boundsMin{ -10.0f, -5.0f };
	Vector2 boundsMax{ 10.0f, 5.0f };
};