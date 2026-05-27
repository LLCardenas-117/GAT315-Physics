#pragma once
#include "raylib.h"

class WorldCamera
{
public:
	// center: screen pixel position of world origin; size: half-height in world units
	WorldCamera(Vector2 center, float size) :
		center{ center },
		size{ size }
	{ }

	void Begin();	// push camera transform before draw
	void End();		// pop camera transform after draw

	Vector2 ScreenToWorld(Vector2 screen); // pixels -> world units
	Vector2 WorldToScreen(Vector2 world);  // world units -> pixels

	// pixels per unit = screen height / (size * 2)
	float GetPixelsPerUnit(); 

private:
	Vector2 center; // screen center in pixels
	float size;		// camera half-height in world units
};
