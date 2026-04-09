#include "raylib.h"
#include "World.h"
#include "Effector.h"

World::World() {
	bodies.reserve(1000);
}

void World::Step(float deltaTime) {
	Vector2 currentMousePosition = GetMousePosition();

	//for (auto& body : bodies) body.acceleration = gravity * body.gravityScale * 100.0f;
	for (auto& body : bodies) body.acceleration = Vector2{ 0, 0 };
	for (auto& body : bodies) body.AddForce(gravity * body.gravityScale * 100.0f, ForceMode::Acceleration);
	//for (auto& body : bodies) body.AddForce(gravity * 100.0f);

	// Attract
	for (auto& effector : effectors) effector->Apply(bodies);
	if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
		for (auto& body : bodies) {
			Vector2 direction = currentMousePosition - body.position;
			if (Vector2Length(direction) <= 100.0f) {
				Vector2 force = Vector2Normalize(direction) * 10000.0f;
				body.AddForce(force);
			}
		}

		DrawCircleLinesV(currentMousePosition, 100, WHITE);
	}

	// Repel
	if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
		for (auto& body : bodies) {
			Vector2 direction = body.position - currentMousePosition;
			if (Vector2Length(direction) <= 100.0f) {
				Vector2 force = Vector2Normalize(direction) * 10000.0f;
				body.AddForce(force);
			}
		}

		DrawCircleLinesV(currentMousePosition, 100, WHITE);
	}

	for (auto& body : bodies) if(body.bodyType == BodyType::Dynamic) body.Step(deltaTime);

	UpdateCollision();

	// force effectors
	for (auto& effector : effectors) effector->Apply(bodies);
}

void World::Draw() {
	//bodies.reserve(1000);
	for (auto& body : bodies) body.Draw();
	for (auto& effector : effectors) effector->Draw();

}

void World::AddBody(const Body& body) {
	bodies.push_back(body);
}

void World::AddEffector(Effector* effector)
{
	effectors.push_back(effector);
}

void World::UpdateCollision()
{
	contacts.clear();
	CreateContacts(bodies, contacts);
	SeparateContacts(contacts);
	ResolveContacts(contacts);

	// Collision
	for (auto& body : bodies) {
		if (body.position.x + body.size > GetScreenWidth()) {
			body.position.x = GetScreenWidth() - body.size;
			body.velocity.x *= -body.restitution;
		}

		if (body.position.x + body.size < 0) {
			body.position.x = 0 + body.size;
			body.velocity.x *= -body.restitution;
		}

		if (body.position.y + body.size > GetScreenHeight()) {
			body.position.y = GetScreenHeight() - body.size;
			body.velocity.y *= -body.restitution;
		}

		if (body.position.y + body.size < 0) {
			body.position.y = 0 + body.size;
			body.velocity.y *= -body.restitution;
		}
	}
}
