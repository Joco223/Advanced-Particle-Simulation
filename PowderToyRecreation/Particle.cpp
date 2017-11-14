#include "Particle.h"

Particle::Particle() {
	position = Vector2D(0, 0);
	velocity = Vector2D(0, 0);
	acceleration = Vector2D(0, 0);
	type = 0;
	mass = 1;
	kinematic = false;
	calculatedD = false;
}

Particle::Particle(const Vector2D& Position, const Vector2D& Velocity, const Vector2D& Acceleration, const int& Type, const bool& Kinematic, const float& Mass, float Heat, float HeatRatio) {
	position = Position;
	velocity = Velocity;
	acceleration = Acceleration;
	type = Type;
	mass = Mass;
	kinematic = Kinematic;
	newVel = Velocity;
	newPos = Position;
	calculatedD = false;
	heat = Heat;
	heatRatio = HeatRatio;
	force = Vector2D(0, 0);
}

void Particle::update(double& deltaTime, float& timeScale) {
	if (this->kinematic == false) {
		force = force.Div(mass);
		float tempf = deltaTime * timeScale;

		force = force.Mlt(tempf);

		Vector2D temp2 = this->acceleration.Mlt(tempf);
		this->newVel = this->newVel.Add(temp2);
		this->newVel = this->newVel.Add(force);
		//this->velocity = this->velocity.Mlt(0.998);

		Vector2D temp = this->newVel.Mlt(tempf);
		this->newPos = this->newPos.Add(temp);

		this->acceleration = Vector2D(0, 0);
		force = Vector2D(0, 0);
	}
}

void Particle::attracted(std::vector<Particle>& others, int id) {
	Vector2D total;

	for (int i = 0; i < others.size(); i++) {
		if (i != id) {
			Vector2D force = this->position.Sub(others[i].position);
			float distance = this->position.Distance(others[i].position);
			float G = 6.67408 * 0.001;
			float strength = (G * ((this->mass * others[i].mass) / distance)) * -1;
			force = force.Mlt(strength);
			total = total.Add(force);
		}	
	}
	
	this->acceleration = this->acceleration.Add(total);


}

void Particle::molecularBond(std::vector<Particle>& others, int id, float bondStr) {
	for (int i = 0; i < others.size(); i++) {
		if (i != id) {
			Vector2D tmp = position.Sub(others[i].position);
			float distance = position.Distance(others[i].position);
			float strength = (bondStr * (1 / (distance * distance)));
			tmp = tmp.Mlt(strength);
			Vector2D vDiff = newVel.Add(others[i].newVel);
			Vector2D vDiff2 = newVel.Sub(others[i].velocity);
			float avg = 2;
			vDiff = vDiff.Div(avg);
			
			if (distance < 1 && (vDiff2.Mag() < tmp.Mag())) {
				newVel = vDiff;
				others[i].newVel = vDiff;
			}	
		}	
	}
}

void Particle::testCollision(double& deltaTime, float& timeScale, std::vector<Particle>& others, int id) {

	for (int i = 0; i < others.size(); i++) {
		if (i != id) {
			float distance = position.Distance(others[i].position);

			if (distance < 1) {
				// get the mtd
				Vector2D delta = (position.Sub(others[i].position));
				float d = delta.Mag();
				// minimum translation distance to push balls apart after intersecting
				Vector2D mtd = delta.Mlt(((0.5 + 0.5) - d) / d); 

				// inverse mass quantities
				float im1 = 1 / mass;
				float im2 = 1 / others[i].mass;

				// push-pull them apart based off their mass
				Vector2D tmp = mtd.Mlt(im1 / (im1 + im2));
				newPos = newPos.Add(tmp); //This here, it seperates them so they are not penetrated anymore

				Vector2D v = (velocity.Sub(others[i].velocity));
				Vector2D tmp2 = mtd.Normal();
				float vn = v.dot(tmp2);

				// collision impulse
				float i = (-(1.0f + 0) * vn) / (im1 + im2);
				Vector2D impulse = mtd.Mlt(i);

				// change in momentum
				Vector2D tmp3 = impulse.Mlt(im1);
				newVel = newVel.Add(tmp3);
			}
		}
	}
	
}

void Particle::calcSpring(std::vector<Particle>& others, float distance, float springC, float dampC, int id, unsigned int* pixels) {

	if (calculatedD == false) {
		distances.resize(others.size());

		for (int i = 0; i < others.size(); i++) {
			if (i != id) {
				float distanceB = (this->position.Distance(others[i].position));

				if (distanceB <= distance) {
					Vector2D temp = position.Sub(others[i].position);
					float temp2 = temp.Mag();
					Vector2D temp3 = velocity.Sub(others[i].velocity);
					Vector2D temp4 = temp.Div(temp2);
					float temp5 = (temp3.dot(temp)) / temp2;
					float temp6 = springC * (distanceB - temp2) - temp5 * dampC;
					Vector2D tmp = temp.Normal();
					Vector2D f = tmp.Mlt(temp6);

					newVel = newVel.Add(f);

					distances[i] = distanceB;
				}			
			}
		}
		calculatedD = true;
	}else if (calculatedD == true) {
		for (int i = 0; i < others.size(); i++) {
			if (i != id) {
				float distanceB = (this->position.Distance(others[i].position));

				if (distanceB < distances[i] * 3) {
					Vector2D temp = position.Sub(others[i].position);
					float temp2 = temp.Mag();
					Vector2D temp3 = velocity.Sub(others[i].velocity);
					Vector2D temp4 = temp.Div(temp2);
					float temp5 = (temp3.dot(temp)) / temp2;
					float temp6 = springC * (distances[i] - temp2) - temp5 * dampC;
					Vector2D tmp = temp.Normal();
					Vector2D f = tmp.Mlt(temp6);

					newVel = newVel.Add(f);
				}
			}
		}
	}
	
}

void Particle::transferHeat(std::vector<Particle>& others, double& deltaTime, float& timeScale) {
	for (int i = 0; i < others.size(); i++) {
		float distance = position.Distance(others[i].position);

		if (distance < 1.6) {
			float e = (heat + others[i].heat) / 2;

			heat += (e - heat) * (deltaTime * timeScale * heatRatio);
			others[i].heat += (e - others[i].heat) * (deltaTime * timeScale * others[i].heatRatio);
		}
	}
}

