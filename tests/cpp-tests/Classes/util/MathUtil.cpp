
#include <math.h>
#include "MathUtil.h"
namespace crop{
namespace MathUtil {
	/**
	 * Calculates the distance between two points (x1, y1) and (x2, y2).
	 */
	float calculateDistance(float x1, float y1, float x2, float y2) {

		float side1 = x2 - x1;
		float side2 = y2 - y1;

		return (float)sqrtf(side1 * side1 + side2 * side2);
	}
}
}
