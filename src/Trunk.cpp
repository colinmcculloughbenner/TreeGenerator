#include "Trunk.h"

Trunk::Trunk(float growthRate, float startingRadius, float positionX, float positionY)
{
	m_growthRate = growthRate;
	m_age = 1;
	m_radius = startingRadius;
	m_position.x = positionX;
	m_position.y = positionY;
}

void Trunk::Grow()
{
	++m_age;
	m_radius += m_growthRate;
}

void Trunk::Draw(Color color) const
{
	DrawCircle((int)m_position.x, (int)m_position.y, m_radius, color);
}
