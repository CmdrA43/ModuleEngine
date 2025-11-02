#include "vectors.h"
#include <cmath>

namespace frame {
	
	//figure out how to actually write a 2d cross product, because they are very confusing
	/*double vector2::crossProduct(vector2& vectorA, vector2& vectorB) {
		return (double(vectorA.x * vectorB.y) - double(vectorB.x * vectorA.y));
	}*/

	double dotProduct(vector2& vectorA, vector2& vectorB) {
		return (double(vectorA.x * vectorB.x) + double(vectorA.y * vectorB.y));
	}

	void normalize(vector3& vector) {
		double magnitude = sqrt((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));
		vector.x /= magnitude;
		vector.y /= magnitude;
		vector.z /= magnitude;
	}

	double dotProduct(vector3& vectorA, vector3& vectorB) {
		return (double(vectorA.x * vectorB.x) + double(vectorA.y * vectorB.y) + double(vectorA.z * vectorB.z));
	}
}