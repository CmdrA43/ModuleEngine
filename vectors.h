#pragma once
#include <cmath>

namespace frame {
	class vector2 {
		public:
		double x, y;

		vector2(double x_val = 0.0, double y_val = 0.0) : x(x_val), y(y_val) {}

		vector2 operator+ (const vector2& other) const{
			return vector2(x + other.x, y + other.y);
		}

		void operator+= (const vector2& other) {
			x += other.x;
			y += other.y;
		}

		vector2 operator- (const vector2& other) const {
			return vector2(x - other.x, y - other.y);
		}

		void operator-= (const vector2& other) {
			x -= other.x;
			y -= other.y;
		}

		vector2 operator* (const vector2& other) const {
			return vector2(x * other.x, y * other.y);
		}

		void operator*= (const vector2& other) {
			x *= other.x;
			y *= other.y;
		}

		void operator*= (const double& other) {
			x *= other;
			y *= other;
		}

		vector2 operator/ (const vector2& other) const {
			return vector2(x / other.x, y / other.y);
		}

		void operator/= (const vector2& other) {
			x /= other.x;
			y /= other.y;
		}

		vector2 operator== (const vector2& other) const {
			return vector2(x == other.x, y == other.y);
		}

		vector2 operator> (const vector2& other) const {
			return vector2(x > other.x, y > other.y);
		}

		vector2 operator< (const vector2& other) const {
			return vector2(x < other.x, y < other.y);
		}

		void normalize() {
			double magnitude = sqrt((x * x) + (y * y));
			x /= magnitude;
			y /= magnitude;
		};

	};

	class vector3 {
	public:
		double x, y, z;

		vector3(double x_val = 0.0, double y_val = 0.0, double z_val = 0.0) : x(x_val), y(y_val), z(z_val) {}

		vector3 operator+ (const vector3& other) const {
			return vector3(x + other.x, y + other.y, z + other.z);
		}

		vector3 operator+ (int other) const {
			return vector3(x + other, y + other, z + other);
		}

		void operator+= (const vector3& other) {
			x += other.x;
			y += other.y;
			z += other.z;
		}

		vector3 operator- (const vector3& other) const {
			return vector3(x - other.x, y - other.y, z - other.z);
		}

		vector3 operator- (int other) const {
			return vector3(x - other, y - other, z - other);
		}

		void operator-= (const vector3& other) {
			x -= other.x;
			y -= other.y;
			z -= other.z;
		}

		vector3 operator* (const vector3& other) const {
			return vector3(x * other.x, y * other.y, z * other.z);
		}

		vector3 operator* (int other) const {
			return vector3(x * other, y * other, z * other);
		}
		
		vector3 operator* (double other) const {
			return vector3(x * other, y * other, z * other);
		}

		void operator*= (const vector3& other) {
			x *= other.x;
			y *= other.y;
			z *= other.z;
		}

		vector3 operator/ (const vector3& other) const {
			return vector3(x / other.x, y / other.y, z / other.z);
		}

		vector3 operator/ (int other) const {
			return vector3(x / other, y / other, z / other);
		}

		void operator/= (const vector3& other) {
			x /= other.x;
			y /= other.y;
			z /= other.z;
		}

		vector3 operator== (const vector3& other) const {
			return vector3(x == other.x, y == other.y, z == other.z);
		}

		vector3 operator> (const vector3& other) const {
			return vector3(x > other.x, y > other.y, z > other.z);
		}

		vector3 operator< (const vector3& other) const {
			return vector3(x < other.x, y < other.y, z < other.z);
		}

		void normalize() {
			double magnitude = sqrt((this->x * this->x) + (this->y * this->y) + (this->z * this->z));
			x /= magnitude;
			y /= magnitude;
			z /= magnitude;
		}

		double magnitude() {
			return sqrt((this->x * this->x) + (this->y * this->y) + (this->z * this->z));
		}

	};
}