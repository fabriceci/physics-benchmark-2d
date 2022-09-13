#pragma once

#include "SDL.h"

struct Color {
	Color() {}
	Color(float r, float g, float b, float a = 1.0f) : r(r), g(g), b(b), a(a) {}

	Color& operator*=(float val);

    Uint32 to_uint32() const;

	float r = 0.0f;
	float g = 0.0f;
	float b = 0.0f;
	float a = 1.0f;

	static Color const BLACK;
	static Color const WHITE;
	static Color const RED;
	static Color const GREEN;
	static Color const BLUE;
};