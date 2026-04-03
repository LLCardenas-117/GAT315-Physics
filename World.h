#pragma once

#include "Body.h"

#include <vector>

class World {
public:
	Vector2 gravity{ 0, 9.81f };
	std::vector<Body> bodies;

public:
	World();
	void Step(float deltaTime);
	void Draw();
	void AddBody(const Body& body);
};