#include "Collision.h"

bool Intersects(const Body& bodyA, const Body& bodyB) {
	float distance = Vector2Distance(bodyA.position, bodyB.position);
	float radius = bodyA.size + bodyB.size;

	return (distance <= radius) ? true : false;
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
				float distance = Vector2Length(direction);

				float radius = bodyA.size + bodyB.size;
				contact.depth = radius - distance;
				contact.normal = Vector2Normalize(direction);

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