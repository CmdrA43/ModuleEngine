#pragma once

#ifndef PHYSICS_H
#define PHYSICS_H

#include "vectors.h"
#include "game.h"
#include <vector>
#include <cmath>

namespace frame {
	const double G = 6.6743E-11;
	const double gravAccel = 0;

	class physicsObj3D {
	public:
		double mass;
		vector3 position;
		vector3 linearVel;
		vector3 rotation;
		vector3 rotationalVel;

		physicsObj3D(double a, vector3 b, vector3 c, vector3 d, vector3 e) : mass(a), position(b), linearVel(c), rotation(d), rotationalVel(e) {}

		void update() {
			if (this->position.z > 0) {
				linearVel.z -= frame::gravAccel * frame::Game::getInstance().deltaTime.count();
			}
			if (this->position.z <= 0) {
				linearVel.z = 0;
				position.z = 0;
			}
			position += linearVel;
			rotation += rotationalVel;
		}
	};

	void resolvePhysics3D(physicsObj3D& a, physicsObj3D& b) {
		vector3 dPos = b.position - a.position;
		double dist = dPos.magnitude();
		double force = (G * a.mass * b.mass) / (dist * dist);
		dPos.normalize();
		vector3 acceleration(dPos.x * (force / a.mass), dPos.y * (force / a.mass), dPos.z * (force / a.mass));
		a.linearVel += (acceleration * Game::getInstance().deltaTime.count());
	}
}
#endif PHYSICS_H