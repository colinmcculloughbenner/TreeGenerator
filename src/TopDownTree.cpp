#include "TopDownTree.h"
#include "Random.h"
#include "raygui.h"

TopDownTree::TopDownTree(float positionX, float positionY)
{
	m_trunk = std::make_unique<Trunk>(m_trunkGrowthRate, m_trunkStartRadius, positionX, positionY);
	m_position.x = positionX;
	m_position.y = positionY;
	m_canopy = std::make_unique<Canopy>(m_position);
}

void TopDownTree::Grow(TreeNodeGrowthParams& growthParams)
{
	++m_age;

	m_trunk->Grow();

	// Grow existing branches, which recursively grow the rest of the nodes in the tree
	for (auto treeNode: m_branches) {
		treeNode->Grow(m_position, 0, growthParams);
	}

	// Add new branches
	if (m_branches.empty()) {
		for (int i = 0; i < m_startingBranches; ++i) {
			AddBranch(growthParams.branchProbability);
		}
	}
	else {
		float remaining = m_newBranchesPerIteration;
		while (remaining > 0) {
			remaining -= Random::UnitInterval();
			if (remaining > 0) {
				AddBranch(growthParams.branchProbability);
			}
		}
	}

	// Grow canopy
	float canopyRadius = 0;
	for (auto branch: m_branches) {
		float farthest = branch->FindFarthestLeaf(m_position);
		if (farthest > canopyRadius) canopyRadius = farthest;
	}
	m_canopy->Grow(canopyRadius);
}

void TopDownTree::Draw(int screenWidth, int screenHeight, TreeNodeRenderParams& renderParams)
{
	ClearBackground(RAYWHITE);
	if (b_drawCanopy) {
		m_canopy->Draw(renderParams.leafColor1);
		// If we draw a simplified canopy, we shouldn't draw individual leaves
		renderParams.drawLeaves = false;
	}
	// branchColor2 is used for the oldest branch segments, so it should be used to draw the trunk.
	m_trunk->Draw(renderParams.branchColor2);
	for (auto treeNode: m_branches) {
		treeNode->Draw(m_position, 0, m_age, m_age, renderParams);
	}
	DrawGUI(Vector2{ (float)screenWidth - 320, 360 });
}

void TopDownTree::Reset()
{
	m_trunk = std::make_unique<Trunk>(1, 2, m_position.x, m_position.y);
	m_age = 1;
	m_canopy = std::make_unique<Canopy>(m_position);
	m_branchAngles.clear();
	for (auto branch: m_branches) {
		delete branch;
	}
	m_branches.clear();
}

void TopDownTree::AddBranch(float branchProbability)
{
	// If there are no branches, put the first one anywhere.
	if (m_branches.empty()) {
		float angle = Random::UniformDistribution(0, 360);
		m_branches.push_back(new TreeNode(angle, 5, m_position, 0, branchProbability));
		m_branchAngles.insert(angle);
		return;
	}

	// If there is one branch, put the next one approximately opposite it.
	if (m_branchAngles.size() == 1) {
		float targetAngle = *m_branchAngles.begin() + 180;
		float angle = Random::NormalDistribution(targetAngle, 30);
		while (angle < 0) angle += 360;
		while (angle >= 360) angle -= 360;
		m_branches.push_back(new TreeNode(angle, 5, m_position, 0, branchProbability));
		m_branchAngles.insert(angle);
		return;
	}

	// In all other cases, find the biggest angle gap between branches and put new branch in that gap.
	// m_branchAngle's iterator goes through the angles in sorted order.
	float prev = *m_branchAngles.rbegin() - 360; // Start with the gap between the largest and smallest angles.
	float biggestGap = 0;
	float biggestGapStart = 0;
	float biggestGapEnd = 0;

	for (float angle: m_branchAngles) {
		float gap = angle - prev;
		if (gap > biggestGap) {
			biggestGap = gap;
			biggestGapStart = prev;
			biggestGapEnd = angle;
		}
		prev = angle;
	}

	float mean = (biggestGapStart + biggestGapEnd) * 0.5F;
	float stdDev = biggestGap * 0.15F;
	float angle = Random::NormalDistribution(mean, stdDev);
	while (angle < 0) angle += 360;
	while (angle >= 360) angle -= 360;
	m_branches.push_back(new TreeNode(angle, 5, m_position, 0, branchProbability));
	m_branchAngles.insert(angle);
}

void TopDownTree::DrawGUI(Vector2 startPosition)
{
	GuiPanel(Rectangle{ startPosition.x - 10, startPosition.y, 320, 280 }, "Top-Down Parameters");
	GuiCheckBox(Rectangle{ startPosition.x, startPosition.y + 40, 20, 20 }, "Draw Canopy (overrides Draw Leaves)",
			&b_drawCanopy);
	GuiSlider(Rectangle{ startPosition.x, startPosition.y + 80, 100, 30 }, nullptr,
			TextFormat("Trunk Starting Radius: %1.2f", m_trunkStartRadius), &m_trunkStartRadius, 0, 10);
	GuiSlider(Rectangle{ startPosition.x, startPosition.y + 120, 100, 30 }, nullptr,
			TextFormat("Trunk Growth Rate: %1.2f", m_trunkGrowthRate), &m_trunkGrowthRate, 0, 10);
	if (GuiSpinner(Rectangle{ startPosition.x, startPosition.y + 160, 100, 30 }, nullptr, &m_startingBranches,
			1, 8, b_startingBranchesEditMode)) {
		b_startingBranchesEditMode = !b_startingBranchesEditMode;
	}
	GuiLabel(Rectangle{ startPosition.x + 105, startPosition.y + 160, 200, 30 }, "Starting Branches");
	GuiSlider(Rectangle{ startPosition.x, startPosition.y + 200, 100, 30 }, nullptr,
			TextFormat("New Branches per Iteration: %1.2f", m_newBranchesPerIteration), &m_newBranchesPerIteration, 0,
			5);
	if (GuiButton(Rectangle{ startPosition.x, startPosition.y + 240, 100, 30 }, "Reset")) {
		ResetGuiParams();
	}
}

void TopDownTree::ResetGuiParams()
{
	b_drawCanopy = true;
	m_trunkStartRadius = 2;
	m_trunkGrowthRate = 2;
	m_startingBranches = 4;
	b_startingBranchesEditMode = false;
	m_newBranchesPerIteration = 1;
}
