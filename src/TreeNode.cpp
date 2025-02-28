#include "TreeNode.h"
#include "raymath.h"
#include "Random.h"
#include <algorithm>

TreeNode::TreeNode(float angle, float startingLength, Vector2 sourcePos, float sourceAngle, float branchProbability)
{
	m_angle = angle;
	m_length = startingLength;
	m_localPosition.x = cos(m_angle * PI / 180) * m_length;
	m_localPosition.y = sin(m_angle * PI / 180) * m_length;
	m_age = 1;
	b_canBranch = Random::Probability(branchProbability);
	m_leafColor = Random::UnitInterval();
	UpdateGlobalPosition(sourcePos, sourceAngle);
}

void TreeNode::Draw(Vector2 sourcePos, float sourceAngle, unsigned int sourceAge, unsigned int maxAge,
		TreeNodeRenderParams& renderParams)
{
	// Draw branch
	Color color = ColorLerp(renderParams.branchColor1, renderParams.branchColor2, (float)m_age / (float)maxAge);
	DrawTaperedLine(
			sourcePos,
			m_globalPosition,
			renderParams.thicknessMultiplier * pow(renderParams.thicknessBase, (float)sourceAge),
			renderParams.thicknessMultiplier * pow(renderParams.thicknessBase, (float)m_age),
			color);

	if (!m_childLeft && !m_childRight) {
		// This is a leaf node, so draw leaf if renderParams.drawLeaves is true
		if (renderParams.drawLeaves) {
			DrawCircleV(m_globalPosition, renderParams.leafSize,
					ColorLerp(renderParams.leafColor1, renderParams.leafColor2, m_leafColor));
		}
	}
	else {
		// Draw child nodes
		if (m_childLeft) {
			m_childLeft->Draw(m_globalPosition, sourceAngle + m_angle, m_age, maxAge, renderParams);
		}

		if (m_childRight) {
			m_childRight->Draw(m_globalPosition, sourceAngle + m_angle, m_age, maxAge, renderParams);
		}
	}
}

void TreeNode::Grow(Vector2 sourcePos, float sourceAngle, TreeNodeGrowthParams& growthParams)
{
	++m_age;
	m_length += growthParams.growthRate;
	m_localPosition.x = cos(m_angle * PI / 180) * m_length;
	m_localPosition.y = sin(m_angle * PI / 180) * m_length;
	UpdateGlobalPosition(sourcePos, sourceAngle);

	// If node can branch, try to grow both children
	// Else, try to grow single child
	if (b_canBranch) {
		if (m_childLeft) {
			m_childLeft->Grow(m_globalPosition, m_globalAngle, growthParams);
		}
		else if (Random::Probability(growthParams.newGrowthProbability)) {
			float angle = Random::NormalDistribution(-growthParams.branchAngle, growthParams.branchAngleStdDev);
			m_childLeft = std::make_unique<TreeNode>(angle, growthParams.startLength, m_globalPosition, m_globalAngle,
					growthParams.branchProbability);
		}

		if (m_childRight) {
			m_childRight->Grow(m_globalPosition, m_globalAngle, growthParams);
		}
		else if (Random::Probability(growthParams.newGrowthProbability)) {
			float angle = Random::NormalDistribution(growthParams.branchAngle, growthParams.branchAngleStdDev);
			m_childRight = std::make_unique<TreeNode>(angle, growthParams.startLength, m_globalPosition,
					m_globalAngle, growthParams.branchProbability);
		}
	}
	else {
		if (m_childLeft) {
			m_childLeft->Grow(m_globalPosition, m_globalAngle, growthParams);
		}
		else if (Random::Probability(growthParams.newGrowthProbability)) {
			float angle = Random::NormalDistribution(0, growthParams.branchAngleStdDev);
			m_childLeft = std::make_unique<TreeNode>(angle, growthParams.startLength, m_globalPosition, m_globalAngle,
					growthParams.branchProbability);
		}
	}

}

float TreeNode::FindFarthestLeaf(Vector2 source)
{
	if (m_childLeft && m_childRight) {
		return std::max(m_childLeft->FindFarthestLeaf(source), m_childRight->FindFarthestLeaf(source));
	}
	else if (m_childLeft) {
		return m_childLeft->FindFarthestLeaf(source);
	}
	else if (m_childRight) {
		return m_childRight->FindFarthestLeaf(source);
	}
	else {
		return Vector2Distance(source, m_globalPosition);
	}
}

void TreeNode::UpdateGlobalPosition(Vector2 sourcePos, float sourceAngle)
{
	m_globalAngle = sourceAngle + m_angle;
	m_globalPosition = sourcePos + Vector2Rotate(m_localPosition, sourceAngle * PI / 180);
}

void TreeNode::DrawTaperedLine(Vector2 src, Vector2 dest, float srcThickness, float destThickness, Color color)
{
	// Thickness direction is perpendicular to the direction of the line
	Vector2 thicknessDirection = Vector2Rotate(Vector2Normalize(Vector2Subtract(dest, src)), 0.5 * PI);

	// Use thickness direction to define vertices of a quad that thickens the line
	Vector2 v1 = Vector2Add(src, Vector2Scale(thicknessDirection, srcThickness));
	Vector2 v2 = Vector2Add(dest, Vector2Scale(thicknessDirection, destThickness));
	Vector2 v3 = Vector2Add(dest, Vector2Scale(thicknessDirection, -destThickness));
	Vector2 v4 = Vector2Add(src, Vector2Scale(thicknessDirection, -srcThickness));

	// Draw quad
	DrawTriangle(v1, v2, v3, color);
	DrawTriangle(v1, v3, v4, color);

	// This circle helps blend this node with its parent.
	DrawCircleV(src, srcThickness, color);
}