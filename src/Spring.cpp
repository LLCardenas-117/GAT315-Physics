#include "Spring.h"
#include "body.h"

void Spring::Apply(float multiplier) {
	Vector2 force = GetSpringForce(bodyA->position, bodyB->position, restLength, stiffness * multiplier);

	// Dampen spring
	// Direction vector from body A to body B (spring axis)
	Vector2 direction = bodyB->position - bodyA->position;
	Vector2 nDirection = Vector2Normalize(direction);

	// Relative velocity of bodies
	Vector2 rv = bodyB->velocity - bodyA->velocity;

	// Higher damping when moving along spring axis, less when moving perpendicular
	float dampingFactor = Vector2DotProduct(nDirection, rv) * damping;

	// Damping force opposes motion along spring axis
	Vector2 dampingForce = nDirection * dampingFactor;
	force -= dampingForce;

	bodyA->AddForce(force * -1.0f);
	bodyB->AddForce(force);
}

void Spring::Draw() {
	DrawLineV(bodyA->position, bodyB->position, WHITE);
}

// Returns the force that pulls B to A
Vector2 Spring::GetSpringForce(Vector2 positionA, Vector2 positionB, float restLength, float stiffness) {

	Vector2 direction = positionB - positionA;

	float length = Vector2Length(direction);

	float displacement = length - restLength;

	// f = -kx (k = stiffness | x = displacement)
	float magnitude = -(displacement * stiffness);

	return Vector2Normalize(direction) * magnitude;
}

