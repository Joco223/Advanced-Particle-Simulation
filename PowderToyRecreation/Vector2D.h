#pragma once


class Vector2D {
public:
	float x, y;

	Vector2D();
	Vector2D(const float&, const float&);

	const Vector2D& Sub(Vector2D& other);
	const Vector2D& Add(Vector2D& other);
	const Vector2D& Div(float& value);
	const Vector2D& Mlt(const float& value);
	const Vector2D& MltV(Vector2D& other);
	const Vector2D& Normal();
	const Vector2D& Round();

	const float& Mag();
	const double& Distance(Vector2D& other);
	const float& dot(Vector2D& other);
};