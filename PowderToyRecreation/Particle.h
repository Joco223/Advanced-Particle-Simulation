#pragma once

#include "Vector2D.h"

#include <vector>
#include <iostream>
#include <random>
#include <math.h>

class Particle {
public:
	Vector2D newPos;
	Vector2D force;
	Vector2D position;
	Vector2D velocity;
	Vector2D acceleration;
	Vector2D newVel;
	std::vector<float> distances;
	bool calculatedD;
	int type;
	float radius = 1;
	float mass;
	float heat;
	float heatRatio;
	bool kinematic;

	Particle();
	Particle(const Vector2D& Position, const Vector2D& Velocity, const Vector2D& Acceleration, const int& Type, const bool& Kinematic, const float& Mass, float Heat, float HeatRatio);

	void update(double& deltaTime, float& timeScale);

	void attracted(std::vector<Particle>& others, int id);

	void molecularBond(std::vector<Particle>& others, int id, float bondStr);

	void testCollision(double& deltaTime, float& timeScale, std::vector<Particle>& others, int id);

	void calcSpring(std::vector<Particle>& others, float distance, float springC, float dampC, int id, unsigned int* pixels);

	void transferHeat(std::vector<Particle>& others, double& deltaTime, float& timeScale);
};