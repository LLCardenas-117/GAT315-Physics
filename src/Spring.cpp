#include "Spring.h"

void Spring::Apply(float multiplier) {
}

void Spring::Draw() {

}

Vector2 Spring::GetSpringForce(Vector2 positionA, Vector2 positionB, float restLength, float stiffness) {

	Vector2 direction = positionB - positionA;

	float length = Vector2Length(direction);

	float displacement = length - restLength;

	// f = -kx (k = stiffness | x = displacement)
	float magnitude = -(displacement * stiffness);

	return Vector2Normalize(direction) * magnitude;
}

