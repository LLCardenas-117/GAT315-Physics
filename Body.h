#pragma once
#include "raymath.h"


enum class BodyType {
	Static, 
	Kinematic,
	Dynamic
};

enum class ForceMode {
	Force,
	Impulse,
	Acceleration,
	VelocityChange
};

struct Body {
public:
	BodyType bodyType = BodyType::Dynamic;
	Vector2 acceleration{ 0, 0 };
	Vector2 position{ 0, 0 };
	Vector2 velocity{ 0, 0 };
	float mass = 1.0f;
	float inverseMass = 1.0f; // 1 / mass
	float size = 1.0f;
	float restitution = 1.0f;
	float gravityScale = 1.0f;
	float damping = 0.0f;

public:
	void Step(float deltaTime);
	void Draw();
	void AddForce(Vector2 force, ForceMode forceMode = ForceMode::Force);
};