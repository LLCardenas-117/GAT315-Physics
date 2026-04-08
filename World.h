#pragma once

//#include "Body.h"
#include "Collision.h"

#include <vector>

class World {
public:
	Vector2 gravity{ 0, 9.81f };
	std::vector<Body> bodies;
	std::vector<class Effector*> effectors;
	std::vector<Contact> contacts;

public:
	World();
	void Step(float deltaTime);
	void Draw();
	void AddBody(const Body& body);
	void AddEffector(Effector* effector);
	void UpdateCollision();
};