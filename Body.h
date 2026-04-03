#pragma once
#include "raymath.h"

struct Body {
public:
	Vector2 acceleration;
	Vector2 position;
	Vector2 velocity;
	float mass;
	float size;
	float restitution;

public:
	void Step(float deltaTime);
	void Draw();
	void AddForce(Vector2 force);
};