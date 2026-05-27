#include "world_camera.h"
#include "rlgl.h"

void WorldCamera::Begin()
{
	// push camera transform
	rlDisableBackfaceCulling();
	rlPushMatrix();

	float ppu = GetPixelsPerUnit();

	// translate to camera offset (screen center)
	rlTranslatef(center.x, center.y, 0.0f);
	// apply scale
	rlScalef(ppu, -ppu, 1.0f); // note: negative Y for flip (+y = up | -y = down)
}

void WorldCamera::End()
{
	// pop camera transform
	rlPopMatrix();
}

Vector2 WorldCamera::ScreenToWorld(Vector2 screen)
{
	float ppu = GetPixelsPerUnit();
	// undo translate, undo scale, undo Y-flip
	return Vector2{	(screen.x - center.x) / ppu, -(screen.y - center.y) / ppu };
}

Vector2 WorldCamera::WorldToScreen(Vector2 world)
{
	float ppu = GetPixelsPerUnit();
	return Vector2{	world.x * ppu + center.x, -world.y * ppu + center.y	};
}

float WorldCamera::GetPixelsPerUnit()
{
	// Converts world units to screen pixels.
	// Example: a screen height of 720 with a camera size of 5
	// results in 72 pixels per world unit (720 / (5 * 2)).
	return GetScreenHeight() / (size * 2);
}