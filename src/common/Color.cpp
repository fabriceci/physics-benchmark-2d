#include "Color.h"
#include <math.h>

Color const Color::BLACK	= Color(0.0f, 0.0f, 0.0f);
Color const Color::WHITE	= Color(1.0f, 1.0f, 1.0f);
Color const Color::RED		= Color(1.0f, 0.0f, 0.0f);
Color const Color::GREEN	= Color(0.0f, 1.0f, 0.0f);
Color const Color::BLUE		= Color(0.0f, 0.0f, 1.0f);

Uint32 Color::to_uint32() const {
	Uint32 c = (Uint8)round(a * 255);
	c <<= 8;
	c |= (Uint8)round(b * 255);
	c <<= 8;
	c |= (Uint8)round(g * 255);
	c <<= 8;
	c |= (Uint8)round(r * 255);

	return c;
}

Color& Color::operator*=(float val) {
	r *= val;
	g *= val;
	b *= val;
	return *this;
}