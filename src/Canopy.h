#ifndef TREEGENERATOR_CANOPY_H
#define TREEGENERATOR_CANOPY_H

#include "raylib.h"
#include<vector>

class Canopy {
public:
	explicit Canopy(Vector2 position)
			:m_position{ position } { };
	void Grow(float radius);
	void Draw(Color color);
private:
	// Number of growth iterations so far
	unsigned int m_age = 1;
	// Position in pixels in screen space
	Vector2 m_position{};
	// Points on the outer edge of the canopy
	std::vector<Vector2> m_perimeter{};
};

#endif //TREEGENERATOR_CANOPY_H
