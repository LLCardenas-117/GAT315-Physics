/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "raylib.h"
#include "raymath.h"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

#include <vector>

struct Body {
	Vector2 acceleration;
	Vector2 position;
	Vector2 velocity;
	float mass;
	float size;
	float restitution;
};

float GetRandomFloat() {
	return GetRandomValue(0, 10000) / (float)10000;
}

void AddForce(Body& body, Vector2 force) {
	body.acceleration += force / body.mass;
}

void ExplicitEuler(Body& body, float deltaTime) {
	body.position += body.velocity * deltaTime;
	body.velocity += body.acceleration * deltaTime;
}

void SemiImplicitEuler(Body& body, float deltaTime) {
	body.velocity += body.acceleration * deltaTime;
	body.position += body.velocity * deltaTime;
}

Vector2 gravity{ 0, 9.81f };

int main ()
{
	std::vector<Body> bodies;
	bodies.reserve(1000);

	SetRandomSeed(5);

	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(1280, 800, "Physics Engine");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// Load a texture from the resources directory
	Texture wabbit = LoadTexture("wabbit_alpha.png");
	
	// game loop
	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		float deltaTime = GetFrameTime();
		Vector2 currentMousePosition = GetMousePosition();

		if (IsKeyDown(KEY_LEFT_SHIFT)) deltaTime = 0.0f;

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || (IsKeyDown(KEY_LEFT_CONTROL) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) ) {
			Body body;
			body.position = currentMousePosition;

			float angle = GetRandomFloat() * (2 * PI);

			// Get random unit circle
			Vector2 direction;
			direction.x = cosf(angle);
			direction.y = sinf(angle);

			body.velocity = direction * (50.0f + (GetRandomFloat() * 300));
			body.acceleration = Vector2{ 0, 0 };
			body.size = 5.0f + (GetRandomFloat() * 20.0f);
			body.restitution = 0.5f + (GetRandomFloat() * 0.5f);
			body.mass = 1.0f;

			bodies.push_back(body);
		}

		// UPDATE
		DrawCircleV(currentMousePosition, 5, SKYBLUE);

		for (auto& body : bodies) body.acceleration = Vector2{ 0, 0 };
		for (auto& body : bodies) AddForce(body, (gravity * 100.0f));

		// Attract
		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
			for (auto& body : bodies) {
				Vector2 direction = currentMousePosition - body.position;
				if (Vector2Length(direction) <= 100.0f) {
					Vector2 force = Vector2Normalize(direction) * 10000.0f;
					AddForce(body, force);
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
					AddForce(body, force);
				}
			}

			DrawCircleLinesV(currentMousePosition, 100, WHITE);
		}

		for (auto& body : bodies) SemiImplicitEuler(body, deltaTime);

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

			/*if (body.position.y + body.size < 0) {
				body.position.y = 0 + body.size;
				body.velocity.y *= -body.restitution;
			}*/
		}

		// DRAW
		BeginDrawing();

		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(BLACK);

		// draw some text using the default font
		DrawText("Physics Engine", 200,200,20,WHITE);

		// draw our texture to the screen
		DrawTexture(wabbit, 400, 200, WHITE);

		for (const auto& body : bodies) {
			DrawCircleV(body.position, body.size, PINK);
		}
		
		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}

	// cleanup
	// unload our texture so it can be cleaned up
	UnloadTexture(wabbit);

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
