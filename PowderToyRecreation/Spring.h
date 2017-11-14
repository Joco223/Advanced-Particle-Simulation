#pragma once

#include "Particle.h"
#include <vector>
#include <iostream>
#include <math.h> 

class Spring {
public:
	int a;
	int b;
	int type;
	float springConstant;
	float dampingConstant;
	float springLength;

	Spring(int, int, int, float, float, float);

	bool operator==(Spring other) {
		if ((a == other.a && b == other.b) || (a == other.b && b == other.a)) { return true; }
		else {
			return false;
		}
	}

	void spawnSprings(std::vector<Particle>& particles, std::vector<Spring>& springs, float springConstant, float dampingConstant, float springLength, int startX, int startY, float bendC, float structC, float sheerC, float bSubH, float bSubW);
	void drawSprings(std::vector<Particle>& particles, std::vector<Spring>& springs, unsigned int* pixels);
	void updateSprings(std::vector<Particle>& particles, std::vector<Spring>& springs, double deltaTime, float timeScale);
};