#ifndef TREEGENERATOR_TOPDOWNTREE_H
#define TREEGENERATOR_TOPDOWNTREE_H

#include "Trunk.h"
#include "TreeNode.h"
#include <list>
#include <set>
#include <memory>
#include "Canopy.h"
#include "Tree.h"

class TopDownTree : public Tree {
public:
	TopDownTree(float positionX, float positionY);
	void Grow(TreeNodeGrowthParams& growthParams) override;
	void Draw(int screenWidth, int screenHeight, TreeNodeRenderParams& renderParams) override;
	void Reset() override;
private:
	// Add a new branch to the tree
	void AddBranch(float branchProbability);
	// Runs GUI logic for Top-Down Tree specific GUI
	void DrawGUI(Vector2 startPosition);
	// Reset the GUI parameters to their default values
	void ResetGuiParams();

	// Tree position in pixels in screen space
	Vector2 m_position{};
	// Number of grow iterations performed so far
	unsigned int m_age = 1;

	// The tree's trunk (rendered as a circle)
	std::unique_ptr<Trunk> m_trunk{};
	// The root nodes of each tree branch
	std::list<TreeNode*> m_branches{};
	// The angles of each branch, used to choose angles of new branches
	std::set<float> m_branchAngles{};
	// A simplified canopy object that grows with the tree's branches
	std::unique_ptr<Canopy> m_canopy{};

	// GUI Parameters
	// Whether to draw a simplified tree canopy. When enabled, drawing leaves is disabled.
	bool b_drawCanopy = true;
	// Starting radius of the tree trunk in pixels
	float m_trunkStartRadius = 2;
	// Trunk radius is incremented by this amount every growth iteration.
	float m_trunkGrowthRate = 2;
	// Number of branches that the tree grows the first growth iteration
	int m_startingBranches = 4;
	// Number of branches to be added on average per iteration. The number of 
	// branches actually added in a given iteration is randomized.
	float m_newBranchesPerIteration = 1;

	// True when the GUI field for m_startingBranches can be edited with the keyboard
	bool b_startingBranchesEditMode = false;
};

#endif //TREEGENERATOR_TOPDOWNTREE_H
