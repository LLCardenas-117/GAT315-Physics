#include "raylib.h"
#include "World.h"
#include "Effector.h"

Vector2 World::gravity = { 0, 9.8f };

Body* World::GetBodyIntersect(Vector2 position)
{
	for (auto& body : bodies) {
		if (CheckCollisionPointCircle(position, body.position, body.size)) return &body;
	}

	return nullptr;
}

World::World() {
	bodies.reserve(1000);
}

void World::Step(float deltaTime) {
	// apply gravity
	for (auto& body : bodies) body.AddForce(gravity * body.gravityScale, ForceMode::Acceleration);

	// force effectors
	for (auto& effector : effectors) effector->Apply(bodies);

	// integrator (acceleration->velocity->position)
	for (auto& body : bodies) if (body.bodyType == BodyType::Dynamic) body.Step(deltaTime);

	for (int iterator = 0; iterator < 4; iterator++) UpdateCollision();

	// reset acceleration
	for (auto& body : bodies) body.acceleration = Vector2{ 0, 0 };
}

void World::Draw() {
	//bodies.reserve(1000);
	for (auto& effector : effectors) effector->Draw();
	for (auto& body : bodies) body.Draw();

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
		if (body.position.x + body.size > boundsMax.x) {
			body.position.x = boundsMax.x - body.size;
			body.velocity.x *= -body.restitution;
		}

		if (body.position.x - body.size < boundsMin.x) {
			body.position.x = boundsMin.x + body.size;
			body.velocity.x *= -body.restitution;
		}

		if (body.position.y + body.size > boundsMax.y) {
			body.position.y = boundsMax.y - body.size;
			body.velocity.y *= -body.restitution;
		}

		if (body.position.y - body.size < boundsMin.y) {
			body.position.y = boundsMin.y + body.size;
			body.velocity.y *= -body.restitution;
		}
	}
}


#pragma region World::Step Legacy
//void World::Step(float deltaTime) {
//	Vector2 currentMousePosition = GetMousePosition();
//
//	//for (auto& body : bodies) body.acceleration = gravity * body.gravityScale * 100.0f;
//	for (auto& body : bodies) body.acceleration = Vector2{ 0, 0 };
//	for (auto& body : bodies) body.AddForce(gravity * body.gravityScale * 100.0f, ForceMode::Acceleration);
//	//for (auto& body : bodies) body.AddForce(gravity * 100.0f);
//
//	// Attract
//	for (auto& effector : effectors) effector->Apply(bodies);
//	if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
//		for (auto& body : bodies) {
//			Vector2 direction = currentMousePosition - body.position;
//			if (Vector2Length(direction) <= 100.0f) {
//				Vector2 force = Vector2Normalize(direction) * 10000.0f;
//				body.AddForce(force);
//			}
//		}
//
//		DrawCircleLinesV(currentMousePosition, 100, WHITE);
//	}
//
//	// Repel
//	if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
//		for (auto& body : bodies) {
//			Vector2 direction = body.position - currentMousePosition;
//			if (Vector2Length(direction) <= 100.0f) {
//				Vector2 force = Vector2Normalize(direction) * 10000.0f;
//				body.AddForce(force);
//			}
//		}
//
//		DrawCircleLinesV(currentMousePosition, 100, WHITE);
//	}
//
//	for (auto& body : bodies) if (body.bodyType == BodyType::Dynamic) body.Step(deltaTime);
//
//	for (int iterator = 0; iterator < 4; iterator++) UpdateCollision();
//
//	// force effectors
//	for (auto& effector : effectors) effector->Apply(bodies);
//}
#pragma endregion
