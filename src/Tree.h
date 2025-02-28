#ifndef TREEGENERATOR_TREE_H
#define TREEGENERATOR_TREE_H

#include "TreeNode.h"

class Tree {
public:
	virtual void Grow(TreeNodeGrowthParams& growthParams) = 0;
	virtual void Draw(int screenWidth, int screenHeight, TreeNodeRenderParams& renderParams) = 0;
	virtual void Reset() = 0;
};

#endif //TREEGENERATOR_TREE_H

