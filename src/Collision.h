#pragma once
#include "Body.h"
#include <vector>

struct Contact
{
	Body* bodyA;
	Body* bodyB;

	float retitution;
	float depth;
	Vector2 normal;
};

void CreateContacts(std::vector<Body>& bodies, std::vector<Contact>& contacts);
void SeparateContacts(std::vector<Contact>& contacts);
void ResolveContacts(std::vector<Contact>& contacts);