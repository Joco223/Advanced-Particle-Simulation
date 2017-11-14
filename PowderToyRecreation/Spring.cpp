#include "Spring.h"

Spring::Spring(int a_, int b_, int type_, float springConstant_, float dampingConstant_, float springLength_) {
	a = a_;
	b = b_;
	type = type_;
	springConstant = springConstant_;
	dampingConstant = dampingConstant_;
	springLength = springLength_;
}

void Spring::spawnSprings(std::vector<Particle>& particles, std::vector<Spring>& springs, float springConstantI, float dampingConstantI, float springLengthI, int startX, int startY, float bendC, float structC, float sheerC, float bSubH, float bSubW) {
	int minX = startX;
	int maxX = 0;
	int minY = startY;
	int maxY = 0;
	std::vector<int> Bendindexes;
	std::vector<Spring> StructuralSprings;
	std::vector<Spring> SheerSprings;
	std::vector<Spring> BendSprings;

	for (int i = 0; i < particles.size(); i++) {
		if (particles[i].position.x > maxX) {
			maxX = particles[i].position.x;
		}
		if (particles[i].position.y > maxY) {
			maxY = particles[i].position.y;
		}
	}

	for (int i = 0; i < particles.size(); i++) {
		std::vector<int> structural;
		std::vector<int> sheer;
		for (int j = 0; j < particles.size(); j++) {
			if (j != i) {
				float distance = particles[j].position.Distance(particles[i].position);

				if (distance <= springLengthI) {
					structural.push_back(j);
				}
				else if (distance <= springLengthI * 1.5) {
					sheer.push_back(j);
				}
			}
		}

		for (int j = 0; j < structural.size(); j++) {
			float distance = particles[structural[j]].position.Distance(particles[i].position);
			Spring temp = Spring(i, structural[j], 2, springConstantI * structC, dampingConstantI * structC, distance);
			bool AE = false;
			for (int k = 0; k < StructuralSprings.size(); k++) {
				if (temp == StructuralSprings[k]) {
					AE = true;
					break;
				}
			}
			if (AE == false) {
				StructuralSprings.push_back(temp);
			}
		}

		for (int j = 0; j < sheer.size(); j++) {
			float distance = particles[sheer[j]].position.Distance(particles[i].position);
			Spring temp = Spring(i, sheer[j], 3, springConstantI * sheerC, dampingConstantI * structC, distance);
			bool AE = false;
			for (int k = 0; k < SheerSprings.size(); k++) {
				if (temp == SheerSprings[k]) {
					AE = true;
					break;
				}
			}
			if (AE == false) {
				SheerSprings.push_back(temp);
			}
		}
	}

	float mX = (maxX - minX) / bSubW;
	float mY = (maxY - minY) / bSubH;

	for (float i = minY; i < maxY + 1; i += mY) {	

		for (float j = minX; j < maxX + 1; j += mX) {

			float pDiff = 100000;
			int id = 0;

			for (int k = 0; k < particles.size(); k++) {
				Vector2D temp = particles[k].position.Sub(Vector2D(j, i));
				if (abs(temp.Mag()) <= pDiff) {
					pDiff = abs(temp.Mag());
					id = k;
				}
			}

			Bendindexes.push_back(id);
		}
	}

	int j = 0;

	for (int k = 0; k < bSubH + 1; k++) {
		for (int i = 0; i < bSubW; i++) {
			float distance = particles[Bendindexes[i + k * bSubW + j]].position.Distance(particles[Bendindexes[i + 1 + k * bSubW + j]].position);
			Spring temp = Spring(Bendindexes[i + k * bSubW + j], Bendindexes[i + 1 + k * bSubW + j], 1, springConstantI * bendC, dampingConstantI * bendC, distance);
			BendSprings.push_back(temp);	
		}

		for (int i = 0; i < bSubW + 1; i++) {
			if (k < bSubH) {
				float distance2 = particles[Bendindexes[i + k * bSubW + j]].position.Distance(particles[Bendindexes[i + 1 + k * bSubW + j + bSubW]].position);
				Spring temp2 = Spring(Bendindexes[i + k * bSubW + j], Bendindexes[i + 1 + k * bSubW + j + bSubW], 1, springConstantI * bendC, dampingConstantI * bendC, distance2);
				BendSprings.push_back(temp2);
			}
		}
		j++;
	}

	for (int i = 0; i < StructuralSprings.size(); i++) {
		springs.push_back(StructuralSprings[i]);
	}

	for (int i = 0; i < SheerSprings.size(); i++) {
		springs.push_back(SheerSprings[i]);
	}

	for (int i = 0; i < BendSprings.size(); i++) {
		springs.push_back(BendSprings[i]);
	}
		
}

void Spring::drawSprings(std::vector<Particle>& particles, std::vector<Spring>& springs, unsigned int* pixels) {

	for (int i = 0; i < springs.size(); i++) {
		float x1 = particles[springs[i].a].position.x;
		float x2 = particles[springs[i].b].position.x;

		float y1 = particles[springs[i].a].position.y;
		float y2 = particles[springs[i].b].position.y;

		float xdiff = (x2 - x1);
		float ydiff = (y2 - y1);

		if(fabs(xdiff) > fabs(ydiff)) {
			float xmin, xmax;

			if(x1 < x2) {
				xmin = x1;
				xmax = x2;
			} else {
				xmin = x2;
				xmax = x1;
			}

			float slope = ydiff / xdiff;
			for(float x = xmin; x <= xmax; x += 1.0f) {
				float y = y1 + ((x - x1) * slope);
				int xP = x;
				int yP = y;
				if (y > 0 && y < 480 - 1) {
					if (x > 0 && x < 640 - 1) {
						if(springs[i].type == 1){ 
							pixels[yP * 640 + xP] = int(0 << 16) | int(0 << 8) | int(255);
						}else if(springs[i].type == 2){ 
							//pixels[yP * 640 + xP] = int(255 << 16) | int(0 << 8) | int(0);
						}else {
							//pixels[yP * 640 + xP] = int(0 << 16) | int(255 << 8) | int(0);
						}
					}
				}
			}
		}else {
			float ymin, ymax;

			if(y1 < y2) {
				ymin = y1;
				ymax = y2;
			} else {
				ymin = y2;
				ymax = y1;
			}

			float slope = xdiff / ydiff;
			for(float y = ymin; y <= ymax; y += 1.0f) {
				float x = x1 + ((y - y1) * slope);
				int xP = x;
				int yP = y;
				if (y > 0 && y < 480 - 1) {
					if (x > 0 && x < 640 - 1) {
						if(springs[i].type == 1){ 
							pixels[yP * 640 + xP] = int(0 << 16) | int(0 << 8) | int(255);
						}else if(springs[i].type == 2){ 
							//pixels[yP * 640 + xP] = int(255 << 16) | int(0 << 8) | int(0);
						}else {
							//pixels[yP * 640 + xP] = int(0 << 16) | int(255 << 8) | int(0);
						}
					}
				}
			}
		}
	}
}

void Spring::updateSprings(std::vector<Particle>& particles, std::vector<Spring>& springs, double deltaTime, float timeScale) {
	for (int i = 0; i < springs.size(); i++) {
		float distanceB = (particles[springs[i].a].position.Distance(particles[springs[i].b].position));

		if (distanceB < springs[i].springLength * 3) {
			Vector2D temp = particles[springs[i].a].position.Sub(particles[springs[i].b].position);
			float temp2 = temp.Mag();
			Vector2D temp3 = particles[springs[i].a].velocity.Sub(particles[springs[i].b].velocity);
			Vector2D temp4 = temp.Div(temp2);
			temp3 = temp3.Normal();
			temp = temp.Normal();
			float temp5 = (temp3.dot(temp)) / temp2;
			float temp6 = (-springs[i].springConstant * (temp2 - springs[i].springLength) - springs[i].dampingConstant * temp5); 
			float hStr = ((particles[springs[i].a].heat + particles[springs[i].b].heat) / 2) * 100;
			Vector2D f = temp4.Mlt(temp6);
			f = f.Div(hStr);
			Vector2D f2 = f.Mlt(-1); 

			float step = deltaTime * timeScale;

			Vector2D tmpV1 = particles[springs[i].a].newVel;
			Vector2D tmpV2 = particles[springs[i].b].newVel;

			Vector2D tmpP1 = particles[springs[i].a].newPos;
			Vector2D tmpP2 = particles[springs[i].b].newPos;

			Vector2D tmpF1 = particles[springs[i].a].force.Add(f);
			tmpF1 = tmpF1.Div(particles[springs[i].a].mass);
			Vector2D tmpF2 = particles[springs[i].b].force.Add(f2);
			tmpF2 = tmpF2.Div(particles[springs[i].b].mass);

			Vector2D tmpA1 = particles[springs[i].a].acceleration.Mlt(step);
			Vector2D tmpA2 = particles[springs[i].b].acceleration.Mlt(step);

			tmpA1 = tmpA1.Add(Vector2D(0, 150.81));
			tmpA2 = tmpA2.Add(Vector2D(0, 150.81));

			tmpV1 = tmpV1.Add(tmpA1);
			tmpV2 = tmpV2.Add(tmpA2);

			tmpV1 = tmpV1.Add(tmpF1);
			tmpV2 = tmpV2.Add(tmpF2);

			tmpV1 = tmpV1.Mlt(0.99998);
			tmpV2 = tmpV2.Mlt(0.99998);

			tmpV1 = tmpV1.Mlt(step);
			tmpV2 = tmpV2.Mlt(step);

			tmpP1 = tmpP1.Add(tmpV1);
			tmpP2 = tmpP2.Add(tmpV2);

			float distanceN = tmpP1.Distance(tmpP2);

			if (distanceB > springs[i].springLength && distanceN < springs[i].springLength) {
				float scale1 = (distanceN / distanceB) * particles[springs[i].a].mass * 0.75;
				float scale2 = (distanceN / distanceB) * particles[springs[i].b].mass * 0.75;
				f = f.Mlt(scale1);
				f2 = f2.Mlt(scale2);
			}else if (distanceB < springs[i].springLength && distanceN > springs[i].springLength) {
				float scale1 = (distanceB / distanceN) * particles[springs[i].a].mass * 0.75;
				float scale2 = (distanceB / distanceN) * particles[springs[i].b].mass * 0.75;
				f = f.Mlt(scale1);
				f2 = f2.Mlt(scale2);
			}		

			particles[springs[i].a].force = particles[springs[i].a].force.Add(f);
			particles[springs[i].b].force = particles[springs[i].b].force.Add(f2);
		}else{
			springs.erase(springs.begin() + i);
		}	
	}
}