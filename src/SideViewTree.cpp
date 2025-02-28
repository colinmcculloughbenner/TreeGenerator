#include "SideViewTree.h"
#include "Random.h"

SideViewTree::SideViewTree(float positionX, float positionY)
{
	m_position.x = positionX;
	m_position.y = positionY;
}

void SideViewTree::Draw(int screenWidth, int screenHeight, TreeNodeRenderParams& renderParams)
{
	ClearBackground(SKYBLUE);
	DrawRectangle(0, screenHeight - 80, screenWidth, 80, renderParams.branchColor2);
	if (m_trunk) {
		m_trunk->Draw(m_position, -90, m_age, m_age, renderParams);
	}
}

void SideViewTree::Grow(TreeNodeGrowthParams& growthParams)
{
	++m_age;
	if (m_trunk) {
		// Grow the root, which recursively grows the rest of the nodes in the tree
		m_trunk->Grow(m_position, -90, growthParams);
	}
	else {
		// Create the root node, randomizing its angle
		float angle = Random::NormalDistribution(0, growthParams.branchAngleStdDev);
		m_trunk = std::make_unique<TreeNode>(angle, growthParams.startLength, m_position, -90,
				growthParams.branchProbability);
	}
}

void SideViewTree::Reset()
{
	m_trunk = nullptr;
	m_age = 1;
}