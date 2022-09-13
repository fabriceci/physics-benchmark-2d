#include "Vector2D.h"
#include <math.h>

Vector2D::Vector2D(): x(0.0), y(0.0) {

}

Vector2D::Vector2D(float x, float y): x(x), y(y) {

}

void Vector2D::Add(const Vector2D& v) {
	x += v.x;
	y += v.y;
}

void Vector2D::Sub(const Vector2D& v) {
	x -= v.x;
	y -= v.y;
}

void Vector2D::Scale(const float n) {
	x *= n;
	y *= n;
}

Vector2D Vector2D::Rotate(const float angle) const {
	Vector2D result;
	result.x = x * (float)cos(angle) - y * (float)sin(angle);
	result.y = x * (float)sin(angle) + y * (float)cos(angle);
	return result;
}

float Vector2D::Magnitude() const {
	return sqrtf(x * x + y * y);
}

float Vector2D::MagnitudeSquared() const {
	return (x * x + y * y);
}

Vector2D& Vector2D::Normalize() {
	float length = Magnitude();
	if (length != 0.0) {
		x /= length;
		y /= length;
	}
    return *this;
}

Vector2D Vector2D::UnitVector() const {
	Vector2D result = Vector2D(0, 0);
	float length = Magnitude();
	if (length != 0.0) {
		result.x = x / length;
		result.y = y / length;
	}
    return result;
}

Vector2D Vector2D::Normal() const {
	return Vector2D(y, -x).Normalize();
}

float Vector2D::Dot(const Vector2D& v) const {
    return (x * v.x) + (y * v.y);
}

float Vector2D::Cross(const Vector2D& v) const {
	return (x * v.y) - (y * v.x);
}

Vector2D& Vector2D::operator = (const Vector2D& v) {
	x = v.x;
	y = v.y;
	return *this;
}

bool Vector2D::operator == (const Vector2D& v) const {
	return x == v.x && y == v.y;
}

bool Vector2D::operator != (const Vector2D& v) const {
	return !(*this == v);
}

Vector2D Vector2D::operator + (const Vector2D& v) const {
	Vector2D result;
	result.x = x + v.x;
	result.y = y + v.y;
	return result;
}

Vector2D Vector2D::operator - (const Vector2D& v) const {
	Vector2D result;
	result.x = x - v.x;
	result.y = y - v.y;
	return result;
}

Vector2D Vector2D::operator * (const float n) const {
	Vector2D result;
	result.x = x * n;
	result.y = y * n;
	return result;
}

Vector2D Vector2D::operator / (const float n) const {
	Vector2D result;
	result.x = x / n;
	result.y = y / n;
	return result;
}

Vector2D& Vector2D::operator += (const Vector2D& v) {
	x += v.x;
	y += v.y;
	return *this;
}

Vector2D& Vector2D::operator -= (const Vector2D& v) {
	x -= v.x;
	y -= v.y;
	return *this;
}

Vector2D& Vector2D::operator *= (const float n) {
	x *= n;
	y *= n;
    return *this;
}

Vector2D& Vector2D::operator /= (const float n) {
	x /= n;
	y /= n;
    return *this;
}

Vector2D Vector2D::operator - () {
	Vector2D result;
	result.x = x * -1;
	result.y = y * -1;
	return result;
}