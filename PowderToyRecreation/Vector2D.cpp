#include "Vector2D.h"

#include <math.h>

Vector2D::Vector2D() {
	x = 0;
	y = 0;
}

Vector2D::Vector2D(const float& X, const float& Y) {
	x = X;
	y = Y;
}

const Vector2D& Vector2D::Sub(Vector2D& other) {
	float X = this->x - other.x;
	float Y = this->y - other.y;
	return Vector2D(X, Y);
}

const Vector2D& Vector2D::Add(Vector2D& other) {
	float X = this->x + other.x;
	float Y = this->y + other.y;
	return Vector2D(X, Y);
}

const Vector2D& Vector2D::Div(float& value) {
	float X = this->x / value;
	float Y = this->y / value;
	return Vector2D(X, Y);
}

const Vector2D& Vector2D::Mlt(const float& value) {
	float X = this->x * value;
	float Y = this->y * value;
	return Vector2D(X, Y);
}

const float& Vector2D::Mag() {
	return sqrt(x * x + y * y);
}

const Vector2D& Vector2D::MltV(Vector2D& other) {
	float X = this->x * other.x;
	float Y = this->y * other.y;
	return Vector2D(X, Y);
}

const Vector2D& Vector2D::Normal() {
	float mag = this->Mag();
	if (mag > 0) {
		return this->Div(mag);
	}else{
		return Vector2D(0, 0);
	}
}

const float& Vector2D::dot(Vector2D& other) {
	return (this->x * other.x) + (this->y * other.y);
}

const double& Vector2D::Distance(Vector2D& other) {
	return sqrt(((this->x - other.x) * (this->x - other.x)) + ((this->y - other.y) * (this->y - other.y)));
}

const Vector2D& Vector2D::Round() {
	float X = ceil(this->x);
	float Y = ceil(this->y);
	return Vector2D(X, Y);
}