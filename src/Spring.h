#pragma once
#include <raymath.h>

struct Body;

class Spring {
public:
	Body* bodyA;
	Body* bodyB;

	float restLength;
	float stiffness;

public:
	Spring() = default;
	Spring(Body* bodyA, Body* bodyB, float restLength, float stiffness = 1) :
		bodyA{bodyA},
		bodyB{bodyB},
		restLength{restLength},
		stiffness{stiffness}
	{}

	void Apply(float multiplier = 1);

	void Draw();

	static Vector2 GetSpringForce(Vector2 positionA, Vector2 positionB, float restLength, float stiffness = 1);
};