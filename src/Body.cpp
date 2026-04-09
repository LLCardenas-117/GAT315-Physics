#include "raylib.h"
#include "Body.h"

void Body::Step(float deltaTime) {
	velocity += acceleration * deltaTime;
	velocity *= (1.0f / (1.0f + damping * deltaTime));
	position += velocity * deltaTime;
}

void Body::Draw() {
	if (bodyType == BodyType::Static) DrawCircleV(position, size, RED);
	else DrawCircleV(position, size, BLUE);
	DrawCircleLinesV(position, size, PINK);
}

void Body::AddForce(Vector2 force, ForceMode forceMode) {
	if (bodyType != BodyType::Dynamic) return;

	switch (forceMode)
	{
	case ForceMode::Force:
		acceleration += force * inverseMass;
		break;

	case ForceMode::Impulse:
		velocity += force * inverseMass;
		break;

	case ForceMode::Acceleration:
		acceleration += force;
		break;

	case ForceMode::VelocityChange:
		velocity += force;
		break;

	default:
		break;
	}
}