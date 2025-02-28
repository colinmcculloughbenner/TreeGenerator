#include "App.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

App::App(int screenWidth, int screenHeight)
{
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	m_topDownTree = std::make_unique<TopDownTree>(screenWidth * 0.5, screenHeight * 0.5);
	m_sideViewTree = std::make_unique<SideViewTree>(screenWidth * 0.5, screenHeight - 80);

	b_isTopDown = false;
	m_tree = m_sideViewTree.get();

	ResetRenderParams();
	ResetGrowthParams();
}

void App::Update()
{
	BeginDrawing();
	m_tree->Draw(m_screenWidth, m_screenHeight, m_renderParams);
	DrawGUI();
	EndDrawing();

}

void App::SwapMode()
{
	if (b_isTopDown) {
		m_tree = m_sideViewTree.get();
	}
	else {
		m_tree = m_topDownTree.get();
	}
	b_isTopDown = !b_isTopDown;
	m_tree->Reset();
}

void App::ResetRenderParams()
{
	m_renderParams.branchColor1 = BROWN;
	m_renderParams.branchColor2 = DARKBROWN;
	m_renderParams.leafColor1 = GREEN;
	m_renderParams.leafColor2 = LIME;
	m_renderParams.leafSize = 3;
	m_renderParams.drawLeaves = true;
	m_renderParams.thicknessBase = 1.2;
	m_renderParams.thicknessMultiplier = 0.5;
}

void App::ResetGrowthParams()
{
	m_growthParams.startLength = 4;
	m_growthParams.growthRate = 2;
	m_growthParams.branchProbability = 0.85;
	m_growthParams.newGrowthProbability = 0.5;
	m_growthParams.branchAngle = 20;
	m_growthParams.branchAngleStdDev = 10;
}

void App::DrawGUI()
{
	// Global GUI
	GuiPanel(Rectangle{ 10, 10, 290, 140 }, nullptr);
	if (GuiButton(Rectangle{ 20, 20, 100, 30 }, "Swap mode")) {
		SwapMode();
	}

	std::string modeText = "Current mode: ";
	if (b_isTopDown) modeText += "Top-Down";
	else modeText += "Side View";
	GuiLabel(Rectangle{ 140, 20, 200, 30 }, modeText.c_str());

	if (GuiButton(Rectangle{ 20, 60, 100, 30 }, "Grow") || IsKeyPressed(KEY_ENTER)) {
		m_tree->Grow(m_growthParams);
	}

	if (GuiButton(Rectangle{ 20, 100, 100, 30 }, "Reset Tree") || IsKeyPressed(KEY_BACKSPACE)) {
		m_tree->Reset();
	}

	// Rendering Parameters
	GuiPanel(Rectangle{ 10, 160, 290, 470 }, "Rendering Parameters");
	GuiCheckBox(Rectangle{ 20, 200, 20, 20 }, "Draw Leaves", &m_renderParams.drawLeaves);
	GuiSlider(Rectangle{ 20, 230, 100, 30 }, nullptr,
			TextFormat("Leaf Size: %1.1f", m_renderParams.leafSize),
			&m_renderParams.leafSize, 0.1, 10);

	GuiSlider(Rectangle{ 20, 285, 100, 30 }, nullptr,
			TextFormat("Branch Thickness Multiplier: %1.2f", m_renderParams.thicknessMultiplier),
			&m_renderParams.thicknessMultiplier, 0.01, 2);
	GuiSlider(Rectangle{ 20, 325, 100, 30 }, nullptr,
			TextFormat("Branch Thickness Base: %1.2f", m_renderParams.thicknessBase),
			&m_renderParams.thicknessBase, 1, 2);

	GuiLabel(Rectangle{ 20, 370, 200, 30 }, "Branch Colors");
	GuiColorPicker(Rectangle{ 20, 390, 80, 80 }, nullptr, &m_renderParams.branchColor1);
	GuiColorPicker(Rectangle{ 140, 390, 80, 80 }, nullptr, &m_renderParams.branchColor2);

	GuiLabel(Rectangle{ 20, 470, 200, 30 }, "Leaf Colors");
	GuiColorPicker(Rectangle{ 20, 490, 80, 80 }, nullptr, &m_renderParams.leafColor1);
	GuiColorPicker(Rectangle{ 140, 490, 80, 80 }, nullptr, &m_renderParams.leafColor2);

	if (GuiButton(Rectangle{ 20, 590, 100, 30 }, "Reset")) {
		ResetRenderParams();
	};

	// Growth Parameters
	float left = (float)m_screenWidth - 320;
	GuiPanel(Rectangle{ left - 10, 20, 320, 320 }, "Growth Parameters");
	GuiSlider(Rectangle{ left, 60, 100, 30 }, nullptr,
			TextFormat("Segment Start Length: %1.2f", m_growthParams.startLength), &m_growthParams.startLength, 0.01,
			10);
	GuiSlider(Rectangle{ left, 100, 100, 30 }, nullptr,
			TextFormat("Growth Rate: %1.2f", m_growthParams.growthRate), &m_growthParams.growthRate, 0.01,
			10);
	GuiSlider(Rectangle{ left, 140, 100, 30 }, nullptr,
			TextFormat("Segment Branch Probability: %1.2f", m_growthParams.branchProbability),
			&m_growthParams.branchProbability, 0,
			1);
	GuiSlider(Rectangle{ left, 180, 100, 30 }, nullptr,
			TextFormat("Segment New Growth Probability: %1.2f", m_growthParams.newGrowthProbability),
			&m_growthParams.newGrowthProbability, 0,
			1);
	GuiSlider(Rectangle{ left, 220, 100, 30 }, nullptr,
			TextFormat("Branch Target Angle: %1.1f\u00B0", m_growthParams.branchAngle),
			&m_growthParams.branchAngle, 0,
			45);
	GuiSlider(Rectangle{ left, 260, 100, 30 }, nullptr,
			TextFormat("Branch Angle Std. Deviation: %1.2f\u00B0", m_growthParams.branchAngleStdDev),
			&m_growthParams.branchAngleStdDev, 0,
			45);
	if (GuiButton(Rectangle{ left, 300, 100, 30 }, "Reset")) {
		ResetGrowthParams();
	};
}