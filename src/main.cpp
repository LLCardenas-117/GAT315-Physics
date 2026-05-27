/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "raylib.h"
#include "raymath.h"

#include "Body.h"
#include "Random.h"
//#include "Integrator.h"
#include "World.h"
#include "world_camera.h"
#include "point_effector.h"
#include "gravitation_effector.h"
#include "Area_Effector.h"
#include "Drag_Effector.h"
#include "Spring.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#define GUI_PHYSICS_IMPLEMENTATION
#pragma warning(push)
#pragma warning(disable: 4576)
#include "gui_physics.h"
#pragma warning(pop)

#include "resource_dir.h" // utility header for SearchAndSetResourceDir
#include <vector>
#include <string>

GuiPhysicsState state;

void AddBody(World& world, Vector2 currentMousePosition);
void AddEffector(World& world, Vector2 currentMousePosition);

int main ()
{
	//world.AddEffector(new GravitationalEffector(30000.0f));
	SetRandomSeed(5);

	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(1280, 800, "Physics Engine");

	// Get GUI state
	state = InitGuiPhysics();
	GuiLoadStyle("raygui-master/styles/terminal/style_terminal.rgs");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	World world;
	WorldCamera world_camera(Vector2{ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f }, 5);
	world.SetBounds(world_camera.ScreenToWorld({ 0, (float)GetScreenHeight() }), world_camera.ScreenToWorld({ (float)GetScreenWidth(), 0 }));
	//world.AddEffector(new PointEffector(Vector2{ 200, 200 }, 100, 30000.0f));


	// Load a texture from the resources directory
	Texture wabbit = LoadTexture("wabbit_alpha.png");

	Body* selectedBody = nullptr;
	Body* connectedBody = nullptr;

	SetTargetFPS(60);

	float timeAccum = 0.0f;
	bool simulate = true;

	// game loop
	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		float deltaTime = fminf(GetFrameTime(), 0.1f);
		float fixedTimeStep = 1.0f / state.FPSValue; // 0.016 * 60.0 = 1.0

		Vector2 currentMousePosition = GetMousePosition();
		Vector2 currentWorldMousePosition = world_camera.ScreenToWorld(currentMousePosition);

		if (IsKeyDown(KEY_LEFT_SHIFT)) deltaTime = GetFrameTime() / 2.0f;
		else if (IsKeyDown(KEY_RIGHT_SHIFT)) deltaTime = GetFrameTime() * 2.0f;
		else if (IsKeyDown(KEY_RIGHT_CONTROL)) deltaTime = GetFrameTime() * 20.0f;
		else if (IsKeyPressed(KEY_SPACE)) state.SimulateActive = !state.SimulateActive;
		else if (IsKeyPressed(KEY_TAB)) state.PhysicsPanelActive = !state.PhysicsPanelActive;

		World::SetGravity(Vector2{ 0.0f, state.GravityValue });

		bool mouseOverGUI = state.PhysicsPanelActive && CheckCollisionPointRec(currentMousePosition, Rectangle{state.anchor01.x, state.anchor01.y, 304, 664});

		if (!mouseOverGUI) {
			if (((IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || (IsKeyDown(KEY_LEFT_CONTROL) && IsMouseButtonDown(MOUSE_BUTTON_LEFT))))) {
				if (IsKeyDown(KEY_LEFT_ALT)) AddEffector(world, currentWorldMousePosition);

				else AddBody(world, currentWorldMousePosition);
			}

			if (selectedBody) {
				if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && IsKeyDown(KEY_LEFT_CONTROL)) {
					Vector2 force = Spring::GetSpringForce(currentWorldMousePosition, selectedBody->position, state.SpringLengthValue, state.SpringStiffnessValue);
					selectedBody->AddForce(force);

					DrawLineV(currentMousePosition, world_camera.WorldToScreen(selectedBody->position), WHITE);
				}
			}

			else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
				selectedBody = world.GetBodyIntersect(currentWorldMousePosition);
			}
		}

		// UPDATE
		DrawCircleV(currentMousePosition, 5, SKYBLUE);

		if (state.SimulateActive) {
			timeAccum += deltaTime;
			while (timeAccum > fixedTimeStep) {
				world.Step(fixedTimeStep);
				timeAccum -= fixedTimeStep;
			}
		}

		// DRAW
		BeginDrawing();

		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(BLACK);

		std::string fpsText = "FPS: ";

		fpsText += std::to_string(GetFPS());

		// draw some text using the default font
		DrawText(fpsText.c_str(), GetScreenWidth() - 120, 50, 20, WHITE);

		world_camera.Begin();
		world.Draw();
		if (IsKeyDown(KEY_LEFT_ALT)) DrawCircleLinesV(currentWorldMousePosition, state.EffectorSizeValue, WHITE);
		else DrawCircleLinesV(currentWorldMousePosition, state.BodySizeValue, WHITE);

		if (selectedBody) DrawCircleLinesV(selectedBody->position, selectedBody->size * 1.05f, PINK);

		world_camera.End();

		// draw our texture to the screen
		//DrawTexture(wabbit, 400, 200, WHITE);

		// Add world draw method here
		//world.Draw();

		GuiPhysics(&state);
		
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

void AddBody(World& world, Vector2 currentMousePosition) {
	Body body;
	body.position = currentMousePosition;

	//body.bodyType = (IsKeyDown(KEY_LEFT_ALT)) ? BodyType::Static : BodyType::Dynamic;
	body.bodyType = (BodyType)state.BodyTypeActive;

	float angle = GetRandomFloat() * (2 * PI);

	// Get random unit circle
	Vector2 direction;
	direction.x = cosf(angle);
	direction.y = sinf(angle);

	//body.AddForce(direction * (50.0f + (GetRandomFloat() * 300)), ForceMode::VelocityChange);
	body.AddForce(direction * state.BodyVelocityValue, ForceMode::VelocityChange);

	//body.size = 5.0f + (GetRandomFloat() * 20.0f);
	body.size = state.BodySizeValue;
	//body.restitution = 0.5f + (GetRandomFloat() * 0.5f);
	body.restitution = state.BodyRestitutionValue;
	//body.mass = body.size;
	body.mass = body.size * state.BodyMassValue;
	body.inverseMass = (body.bodyType == BodyType::Static) ? 0 : 1.0f / body.mass;
	//body.gravityScale = 0.5f;
	//body.gravityScale = 0.0f;
	body.gravityScale = state.BodyGravityValue;
	//body.damping = 0.25f;
	body.damping = state.BodyDampingValue;

	world.AddBody(body);
}

void AddEffector(World& world, Vector2 currentMousePosition) {
	//world.AddEffector(new PointEffector(Vector2{ 1000, 650 }, 100, -30000.0f)); // RED
	//world.AddEffector(new GravitationalEffector(Vector2{ 300, 650 }, 100, 30000.0f)); // PINK
	//world.AddEffector(new AreaEffector(Vector2{ 300, 150 }, 100, 0, 30000.0f)); // GREEN
	//world.AddEffector(new DragEffector(Vector2{ 1000, 150 }, 100, 50.5f, 30000.0f)); // PURPLE
	Effector* effector = nullptr;

	switch ((EffectorType)state.EffectorTypeActive)
	{
	case EffectorType::Gravitation:
		effector = new GravitationalEffector(currentMousePosition, state.EffectorSizeValue, state.EffectorForceValue);
		break;

	case EffectorType::Point:
		effector = new PointEffector(currentMousePosition, state.EffectorSizeValue, state.EffectorForceValue);
		break;

	case EffectorType::Area:
		effector = new AreaEffector(currentMousePosition, state.EffectorSizeValue, state.EffectorAngleValue, state.EffectorForceValue);
		break;

	case EffectorType::Drag:
		effector = new DragEffector(currentMousePosition, state.EffectorSizeValue, state.EffectorAngleValue, state.EffectorForceValue);
		break;

	default:
		break;
	}

	if (effector) world.AddEffector(effector);
	
}