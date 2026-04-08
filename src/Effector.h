#pragma once
#include "../body.h"
#include <vector>

class Effector {
public:
	virtual void Apply(std::vector<Body>& bodies) = 0;
	virtual void Draw() {}
};