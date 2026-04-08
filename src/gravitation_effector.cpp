#include "gravitation_effector.h"

void GravitationalEffector::Apply(std::vector<Body>& bodies)
{
    for (size_t i = 0; i < bodies.size(); i++)
    {
        for (size_t j = i + 1; j < bodies.size(); j++)
        {
            Body& bodyA = bodies[i];
            Body& bodyB = bodies[j];

            // STEP 1: Direction vector
            Vector2 direction = bodyA.position - bodyB.position;

                // STEP 2: Distance
            float distance = Vector2Length(direction);

                // STEP 3: Clamp distance
            distance = fmaxf(distance, 1.0f);

                // STEP 4: Force magnitude
            float forceMagnitude = strength * ((bodyA.mass * bodyB.mass) / (distance * distance));

            // STEP 5: Force vector
            Vector2 force = Vector2Normalize(direction) * forceMagnitude;

                // STEP 6: Apply equal and opposite forces
            bodyA.AddForce(force * -1);
            bodyB.AddForce(force);
        }
    }
}
