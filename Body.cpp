#include "raylib.h"
#include "Body.h"

void Body::Step(float deltaTime) {
	velocity += acceleration * deltaTime;
	position += velocity * deltaTime;
}

void Body::Draw() {
	DrawCircleV(position, size, PINK);
}

void Body::AddForce(Vector2 force) {
	acceleration += force / mass;
}