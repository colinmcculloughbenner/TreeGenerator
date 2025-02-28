#ifndef TREEGENERATOR_SIDEVIEWTREE_H
#define TREEGENERATOR_SIDEVIEWTREE_H

#include "TreeNode.h"
#include "Tree.h"
#include <memory>

class SideViewTree : public Tree {
public:
	SideViewTree(float positionX, float positionY);
	void Grow(TreeNodeGrowthParams& growthParams) override;
	void Draw(int screenWidth, int screenHeight, TreeNodeRenderParams& renderParams) override;
	void Reset() override;
private:
	// Tree position in pixels in screen space
	Vector2 m_position{};
	// The root node of the tree
	std::unique_ptr<TreeNode> m_trunk{};
	// Number of grow iterations performed so far
	unsigned int m_age = 1;
};

#endif //TREEGENERATOR_SIDEVIEWTREE_H
