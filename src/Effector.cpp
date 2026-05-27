#include <raylib.h>
#include "Effector.h"

void Effector::Draw() {
	DrawCircleLinesV(position, size, WHITE);
}

void Effector::CollectBodiesInside(std::vector<Body>& ibodies, std::vector<Body*>& obodies) {
	for (auto& body : ibodies) {
		//if (Vector2Distance(body.position, position) < size) obodies.push_back(&body);
		if (Vector2DistanceSqr(body.position, position) < (size * size)) obodies.push_back(&body);
	}

	//for (auto& body : obodies) body->gravityScale = 0.0f;
}