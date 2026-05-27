#pragma once
#include "Effector.h"

class AreaEffector : public Effector {
public:
	AreaEffector(Vector2 position, float size, float angle, float forceMagnitude) :
		Effector{ position, size },
		angle{ angle },
		forceMagnitude{ forceMagnitude }
	{
	}

	void Apply(std::vector<Body>& bodies) override;
	void Draw() override;

private:
	float angle;
	float forceMagnitude;
};