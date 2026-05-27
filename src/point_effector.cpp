#include <raylib.h>
#include "point_effector.h"

void PointEffector::Apply(std::vector<Body>& ibodies)
{
	std::vector<Body*> bodies;
	CollectBodiesInside(ibodies, bodies);

	for (auto& body : bodies) {
		Vector2 direction = body->position - position;
		Vector2 force = Vector2Normalize(direction) * forceMagnitude;
		body->AddForce(force);
	}
}

void PointEffector::Draw() {
	Effector::Draw();
	DrawCircleV(position, size, Fade(RED, 0.2f));
}