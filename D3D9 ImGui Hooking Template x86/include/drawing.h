#pragma once
#include <cmath>

#include "dx.h"
#include "../classes/vec.h"

class Drawing {
public:
	#define W2S(pos, screen) worldToScreen(pos, screen, d3d9.windowWidth, d3d9.windowHeight)

	float viewMatrix[16];

	bool worldToScreen(Vec3 pos, Vec2& screen, int windowWidth, int windowHeight);

	void drawFilledRect(int x, int y, int w, int h, D3DCOLOR color);

	void drawLine(int x1, int y1, int x2, int y2, int thickness, D3DCOLOR color);

	void drawLine(Vec2 src, Vec2 dst, int thickness, D3DCOLOR color);

	void drawTextOnScreen(const char* text, int x, int y, D3DCOLOR color);

	void drawESPBox2D(Vec2 top, Vec2 bottom, int thickness, D3DCOLOR color);

	void drawESPBox3D(const Vec3& head, const Vec3& feet, int thickness, float width, D3DCOLOR color); //without rotation angle of cped

	void drawDirectionLine(const Vec3& pos, float yaw, float length, D3DCOLOR color);

	void updateViewMatrix();

	Drawing();
};

extern Drawing drawing;

