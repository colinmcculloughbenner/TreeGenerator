#ifndef TREEGENERATOR_TREENODE_H
#define TREEGENERATOR_TREENODE_H
#include "raylib.h"
#include <memory>

// Parameters sent to the Draw and Grow methods
struct TreeNodeRenderParams;
struct TreeNodeGrowthParams;

class TreeNode {
public:
	TreeNode(float angle, float startingLength, Vector2 sourcePos, float sourceAngle, float branchProbability);
	// Draw node and its children, recursively
	void Draw(Vector2 sourcePos, float sourceAngle, unsigned int sourceAge, unsigned int maxAge,
			TreeNodeRenderParams& renderParams);
	// Grow node and its children, recursively
	void Grow(Vector2 sourcePos, float sourceAngle, TreeNodeGrowthParams& growthParams);
	// Needed by TopDownTree to determine canopy radius. Returns the distance from source of
	// the farthest leaf in the tree of which this node is the root.
	float FindFarthestLeaf(Vector2 source);
private:
	// The Draw method needs the global position
	void UpdateGlobalPosition(Vector2 sourcePos, float sourceAngle);
	// Helper function for the Draw method. Each node is drawn as a line with tapered width.
	static void DrawTaperedLine(Vector2 src, Vector2 dest, float srcThickness, float destThickness, Color color);

	// Angle of this node relative to the angle of its parent
	float m_angle;
	// Distance of this node in pixels from its parent
	float m_length;
	// Position of this node in pixels relative to its parent
	Vector2 m_localPosition{};
	// Position of this node in pixels in screen space
	Vector2 m_globalPosition{};
	// Angle of the line from this node's parent to this node in screen space
	float m_globalAngle;

	// The left-branching child of this node or the only child of this node if b_canBranch is false
	std::unique_ptr<TreeNode> m_childLeft{};
	// The right-branching child of this node
	std::unique_ptr<TreeNode> m_childRight{};

	// How many growth iterations this node has had so far
	unsigned int m_age;
	// If true, the node can have two children; otherwise, only it can have only one.
	bool b_canBranch;
	// Value between 0 and 1, used to lerp between the two leaf colors chosen in the GUI if this node is a leaf.
	float m_leafColor;
};

struct TreeNodeRenderParams {
	// Color of youngest branches
	Color branchColor1 = BROWN;
	// Color of oldest branches
	Color branchColor2 = DARKBROWN;
	// Leaf colors are chosen by lerping between leafColor1 and leafColor2
	Color leafColor1 = GREEN;
	Color leafColor2 = LIME;
	// Whether leaves should be rendered
	bool drawLeaves = true;
	// Radius in pixels of leaves
	float leafSize = 3;
	// Node thickness is thicknessMultiplier * thicknessBase ^ m_age
	float thicknessBase = 1.2;
	float thicknessMultiplier = 0.5;
};

struct TreeNodeGrowthParams {
	// Length of node when created
	float startLength = 4;
	// Amount by which the node's length grows per growth iteration
	float growthRate = 3;
	// Probability of new child nodes' being allowed to branch
	float branchProbability = 0.85;
	// Probability that a new child grows if a slot is open in a given growth iteration
	float newGrowthProbability = 0.5;
	// Target local angle of any newly created nodes, when b_CanBranch is true
	float branchAngle = 20;
	// Standard deviation used to randomly choose a node's actual angle value
	float branchAngleStdDev = 10;
};

#endif //TREEGENERATOR_TREENODE_H
