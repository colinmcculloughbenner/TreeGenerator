#ifndef TREEGENERATOR_APP_H
#define TREEGENERATOR_APP_H

#include "TopDownTree.h"
#include "SideViewTree.h"
#include <memory>
#include "TreeNode.h"

class App {
public:
	App(int screenWidth, int screenHeight);
	// Runs tree growth, rendering, and GUI logic every frame
	void Update();
private:
	// Swap between Top-Down and Side View modes
	void SwapMode();
	// Reset the render parameters exposed to the GUI to their default values
	void ResetRenderParams();
	// Reset the tree growth parameters exposed to the GUI to their default values
	void ResetGrowthParams();
	// All global GUI logic is run here, but specific tree types can control additional GUI items
	void DrawGUI();

	// Pointer to either m_topDownTree or m_sideViewTree depending on the value of b_isTopDown
	Tree* m_tree;
	std::unique_ptr<TopDownTree> m_topDownTree;
	std::unique_ptr<SideViewTree> m_sideViewTree;
	bool b_isTopDown;

	int m_screenHeight;
	int m_screenWidth;

	// Tree rendering parameters exposed to the GUI
	TreeNodeRenderParams m_renderParams{};
	// Tree growth parameters exposed to the GUI
	TreeNodeGrowthParams m_growthParams{};
};

#endif //TREEGENERATOR_APP_H
