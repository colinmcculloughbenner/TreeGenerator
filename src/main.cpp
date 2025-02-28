#include"raylib.h"
#include "App.h"

int screenWidth = 1280;
int screenHeight = 720;

int main()
{
	InitWindow(screenWidth, screenHeight, "Tree Generator");
	SetTargetFPS(60);

	App app(screenWidth, screenHeight);
	while (!WindowShouldClose()) {
		app.Update();
	}

	CloseWindow();
	return 0;
}