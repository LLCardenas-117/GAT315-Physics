#include "Random.h"
#include "Collision.h"

bool Intersects(const Body& bodyA, const Body& bodyB) {
	float distanceSqr = Vector2DistanceSqr(bodyA.position, bodyB.position);
	float radius = bodyA.size + bodyB.size;

	return (distanceSqr <= (radius * radius)) ? true : false;
}

void CreateContacts(std::vector<Body>& bodies, std::vector<Contact>& contacts) {
	for (int mainIterator = 0; mainIterator < bodies.size(); mainIterator++) {
		for (int secondaryIterator = mainIterator + 1; secondaryIterator < bodies.size(); secondaryIterator++) {
			Body& bodyA = bodies[mainIterator];
			Body& bodyB = bodies[secondaryIterator];

			if (bodyA.bodyType != BodyType::Dynamic && bodyB.bodyType != BodyType::Dynamic) continue;

			if (Intersects(bodyA, bodyB)) {
				Contact contact;
				contact.bodyA = &bodyA;
				contact.bodyB = &bodyB;

				Vector2 direction = bodyA.position - bodyB.position;
				float distanceSqr = Vector2LengthSqr(direction);

				if (distanceSqr <= EPSILON) {
					direction = Vector2{ GetRandomFloat(-0.05f, 0.05f), GetRandomFloat(-0.05f, 0.05f) };
					distanceSqr = Vector2LengthSqr(direction);
				}

				float distance = sqrtf(distanceSqr);
				float radius = bodyA.size + bodyB.size;
				contact.depth = radius - distance;
				contact.normal = Vector2Normalize(direction);
				contact.retitution = (bodyA.restitution + bodyB.restitution) * 0.5f;

				contacts.push_back(contact);
			}
		}
	}
}

void SeparateContacts(std::vector<Contact>& contacts) {
	for (auto& contact : contacts) {
		float totalInverseMass = contact.bodyA->inverseMass + contact.bodyB->inverseMass;
		Vector2 separation = contact.normal * (contact.depth / totalInverseMass);
		contact.bodyA->position = contact.bodyA->position + (separation * contact.bodyA->inverseMass);
		contact.bodyB->position = contact.bodyB->position - (separation * contact.bodyB->inverseMass);
	}
}

void ResolveContacts(std::vector<Contact>& contacts) {
	for (auto& contact : contacts) {
		Vector2 relativeVelocity = contact.bodyA->velocity - contact.bodyB->velocity;

		float normalVelocity = Vector2DotProduct(relativeVelocity, contact.normal);

		if (normalVelocity > 0) continue;

		float totalInverseMass = contact.bodyA->inverseMass + contact.bodyB->inverseMass;
		float impulseMagnitude = ((1 + contact.retitution) * -1) * normalVelocity / totalInverseMass;

		Vector2 impulse = contact.normal * impulseMagnitude;

		contact.bodyA->AddForce(impulse, ForceMode::Impulse);
		contact.bodyB->AddForce((impulse * -1.0f), ForceMode::Impulse);
	}
}