#pragma once

#include "raylib.h"

inline float GetRandomFloat() {
	return GetRandomValue(0, 10000) / (float)10000;
}

inline float GetRandomFloat(float max) {
	return GetRandomValue(0, max) / max;
}

inline float GetRandomFloat(float min, float max) {
	return GetRandomValue(min, max) / max;
}