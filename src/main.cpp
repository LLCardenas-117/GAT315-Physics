/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "raylib.h"
#include "raymath.h"
#include "resource_dir.h" // utility header for SearchAndSetResourceDir

#include <string>

#include "Body.h"
#include "World.h"
#include "Random.h"
#include "point_effector.h"
#include "gravitation_effector.h"

int main ()
{
	World world;
	//world.AddEffector(new PointEffector(Vector2{ 200, 200 }, 100, 30000.0f));
	//world.AddEffector(new PointEffector(Vector2{ 600, 600 }, 100, -30000.0f));
	//world.AddEffector(new GravitationalEffector(10000.0f));
	SetRandomSeed(5);

	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(1280, 800, "Physics Engine");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// Load a texture from the resources directory
	Texture wabbit = LoadTexture("wabbit_alpha.png");

	SetTargetFPS(60);

	float timeAccum = 0.0f;
	float fixedTimeStep = 1.0f / 60.0f; // 0.016 * 60.0 = 1.0

	
	// game loop
	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		float deltaTime = GetFrameTime();
		Vector2 currentMousePosition = GetMousePosition();

		if (IsKeyDown(KEY_LEFT_SHIFT)) deltaTime = 0.0f;
		else if (IsKeyDown(KEY_RIGHT_CONTROL)) deltaTime = GetFrameTime() / 2.0f;
		else if (IsKeyDown(KEY_RIGHT_SHIFT)) deltaTime = GetFrameTime() * 20.0f;

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || (IsKeyDown(KEY_LEFT_CONTROL) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) ) {
			Body body;
			body.position = currentMousePosition;

			body.bodyType = (IsKeyDown(KEY_LEFT_ALT)) ? BodyType::Static : BodyType::Dynamic;

			float angle = GetRandomFloat() * (2 * PI);

			// Get random unit circle
			Vector2 direction;
			direction.x = cosf(angle);
			direction.y = sinf(angle);

			body.AddForce(direction * (50.0f + (GetRandomFloat() * 300)), ForceMode::VelocityChange);

			body.size = 5.0f + (GetRandomFloat() * 20.0f);
			body.restitution = 0.5f + (GetRandomFloat() * 0.5f);
			body.mass = body.size;
			body.inverseMass = (body.bodyType == BodyType::Static) ? 0 : 1.0f / body.mass;
			//body.gravityScale = 0.5f;
			body.gravityScale = 0.0f;
			body.damping = 0.25f;

			world.AddBody(body);
		}

		// UPDATE
		DrawCircleV(currentMousePosition, 5, SKYBLUE);

		timeAccum += deltaTime;
		while (timeAccum > fixedTimeStep) {
			world.Step(fixedTimeStep);
			timeAccum -= fixedTimeStep;
		}

		// DRAW
		BeginDrawing();

		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(BLACK);

		std::string fpsText = "FPS: ";

		fpsText += std::to_string(GetFPS());

		// draw some text using the default font
		DrawText(fpsText.c_str(), 100, 100, 20, WHITE);

		// draw our texture to the screen
		//DrawTexture(wabbit, 400, 200, WHITE);

		// Add world draw method here
		world.Draw();
		
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
