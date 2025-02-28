#include "Canopy.h"
#include "Random.h"
#include "raymath.h"

void Canopy::Grow(float radius)
{
	++m_age;

	// Recalculate perimeter points based on new age and radius.
	// The points are drawn in a circle with a random offset per point.
	unsigned int numPoints = m_age * 16;
	m_perimeter.clear();
	float anglePerPoint = 2 * PI / (float)numPoints;
	for (int i = 0; i < numPoints; ++i) {
		float angle = anglePerPoint * (float)i;
		Vector2 position{ Random::UniformDistribution(radius - 8, radius + 8), 0 };
		m_perimeter.push_back(Vector2Add(m_position, Vector2Rotate(position, angle)));
	}
}

void Canopy::Draw(Color color)
{
	if (m_perimeter.size() < 3) return;

	for (int i = 0; i < m_perimeter.size(); ++i) {
		Vector2& from = m_perimeter[i];
		Vector2& to = m_perimeter[(i + 1) % m_perimeter.size()];
		// Fill inside the canopy
		DrawTriangle(m_position, to, from, color);
		// Draw an outline around the canopy
		DrawLineEx(from, to, 2, DARKGREEN);
	}

}
