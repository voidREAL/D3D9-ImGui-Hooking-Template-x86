#pragma once
#include <cmath>

#include "dx.h"
#include "../classes/vec.h"

class Drawing {
public:
	#define W2S(pos, screen) worldToScreen(pos, screen, d3d9.windowWidth, d3d9.windowHeight)

	float viewMatrix[16];

	bool worldToScreen(Vec3 pos, Vec2& screen, int windowWidth, int windowHeight);

	void DrawFilledRect(int x, int y, int w, int h, D3DCOLOR color);

	void DrawLine(int x1, int y1, int x2, int y2, int thickness, D3DCOLOR color);

	void DrawLine(Vec2 src, Vec2 dst, int thickness, D3DCOLOR color);

	void DrawTextOnScreen(const char* text, int x, int y, D3DCOLOR color);

	void DrawESPBox2D(Vec2 top, Vec2 bottom, int thickness, D3DCOLOR color);

	void DrawESPBox3D(const Vec3& head, const Vec3& feet, int thickness, float width, D3DCOLOR color); //without rotation angle of cped

	void DrawDirectionLine(const Vec3& pos, float yaw, float length, D3DCOLOR color);

	Drawing();
};

extern Drawing drawing;

