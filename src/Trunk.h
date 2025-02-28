#ifndef TREEGENERATOR_TRUNK_H
#define TREEGENERATOR_TRUNK_H

#include "raylib.h"

class Trunk {
public:
	Trunk(float growthRate, float startingRadius, float positionX, float positionY);
	void Grow();
	void Draw(Color color) const;
private:
	// Number of growth iterations so far
	int m_age;
	// Radius of trunk in pixels
	float m_radius;
	// How much radius grows each iteration
	float m_growthRate;
	// Position of trunk in pixels in screen space
	Vector2 m_position{};
};

#endif //TREEGENERATOR_TRUNK_H
