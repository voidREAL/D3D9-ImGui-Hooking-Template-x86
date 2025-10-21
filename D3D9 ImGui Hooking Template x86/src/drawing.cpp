#include "../include/drawing.h"
#define M_PI 3.14159265358979323846

Drawing drawing;

void Drawing::drawFilledRect(int x, int y, int w, int h, D3DCOLOR color) {
	D3DRECT rect = { x, y, x + w, y + h };
	d3d9.pDevice->Clear(1, &rect, D3DCLEAR_TARGET, color, 0, 0);
}

void Drawing::drawLine(int x1, int y1, int x2, int y2, int thickness, D3DCOLOR color)
{
	if (!d3d9.lineL) {
		D3DXCreateLine(d3d9.pDevice, &d3d9.lineL);
	}

	D3DXVECTOR2 Line[2];
	Line[0] = D3DXVECTOR2(x1, y1);
	Line[1] = D3DXVECTOR2(x2, y2);
	if (d3d9.lineL != nullptr) {
		d3d9.lineL->SetWidth(thickness);
		d3d9.lineL->Draw(Line, 2, color);
	}
}

void Drawing::drawLine(Vec2 src, Vec2 dst, int thickness, D3DCOLOR color) {
	drawLine(src.x, src.y, dst.x, dst.y, thickness, color);
}

void Drawing::drawTextOnScreen(const char* text, int x, int y, D3DCOLOR color) {
	if (!d3d9.font) {
		D3DXCreateFontA(
			d3d9.pDevice,
			14,
			0,
			FW_BOLD,
			1,
			FALSE,
			DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS,
			ANTIALIASED_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE,
			"Tahoma",
			&d3d9.font
		);
	}

	if (!d3d9.font) return;

	RECT rect;
	SetRect(&rect, x + 1, y + 1, x + 1, y + 1);
	if (d3d9.font) {
		d3d9.font->DrawTextA(nullptr, text, -1, &rect, DT_CENTER | DT_NOCLIP, D3DCOLOR_ARGB(255, 0, 0, 0));
		SetRect(&rect, x, y, x, y);
		d3d9.font->DrawTextA(nullptr, text, -1, &rect, DT_CENTER | DT_NOCLIP, color);
	}
}

void Drawing::drawESPBox2D(Vec2 top, Vec2 bottom, int thickness, D3DCOLOR color) {
	int height = abs(top.y - bottom.y);
	Vec2 tl, tr;
	tl.x = top.x - (height / 4);
	tr.x = top.x + (height / 4);
	tl.y = tr.y = top.y;

	Vec2 bl, br;
	bl.x = bottom.x - (height / 4);
	br.x = bottom.x + (height / 4);
	bl.y = br.y = bottom.y;

	drawLine(tl, tr, thickness, color);
	drawLine(tr, br, thickness, color);
	drawLine(bl, br, thickness, color);
	drawLine(tl, bl, thickness, color);
}

void Drawing::drawESPBox3D(const Vec3& head, const Vec3& feet, int thickness, float width, D3DCOLOR color) // without rotation angle
{
	Vec3 up = { head.x - feet.x, head.y - feet.y, head.z - feet.z };

	Vec3 right = { -up.y, up.x, 0.0f };
	float len = sqrtf(right.x * right.x + right.y * right.y + right.z * right.z);
	if (len > 0.001f) { right.x /= len; right.y /= len; right.z /= len; }

	Vec3 forward = {
		up.y * right.z - up.z * right.y,
		up.z * right.x - up.x * right.z,
		up.x * right.y - up.y * right.x
	};
	len = sqrtf(forward.x * forward.x + forward.y * forward.y + forward.z * forward.z);
	if (len > 0.001f) { forward.x /= len; forward.y /= len; forward.z /= len; }

	float hw = width * 0.5f;

	Vec3 p[8];
	p[0] = { head.x + right.x * hw + forward.x * hw, head.y + right.y * hw + forward.y * hw, head.z + right.z * hw + forward.z * hw };
	p[1] = { head.x - right.x * hw + forward.x * hw, head.y - right.y * hw + forward.y * hw, head.z - right.z * hw + forward.z * hw };
	p[2] = { head.x + right.x * hw - forward.x * hw, head.y + right.y * hw - forward.y * hw, head.z + right.z * hw - forward.z * hw };
	p[3] = { head.x - right.x * hw - forward.x * hw, head.y - right.y * hw - forward.y * hw, head.z - right.z * hw - forward.z * hw };
	p[4] = { feet.x + right.x * hw + forward.x * hw, feet.y + right.y * hw + forward.y * hw, feet.z + right.z * hw + forward.z * hw };
	p[5] = { feet.x - right.x * hw + forward.x * hw, feet.y - right.y * hw + forward.y * hw, feet.z - right.z * hw + forward.z * hw };
	p[6] = { feet.x + right.x * hw - forward.x * hw, feet.y + right.y * hw - forward.y * hw, feet.z + right.z * hw - forward.z * hw };
	p[7] = { feet.x - right.x * hw - forward.x * hw, feet.y - right.y * hw - forward.y * hw, feet.z - right.z * hw - forward.z * hw };

	Vec2 s[8];
	for (int i = 0; i < 8; i++)
		if (worldToScreen(p[i], s[i], d3d9.windowWidth, d3d9.windowHeight)) {
			return;
		}

	auto L = [&](int a, int b) {
		drawLine(s[a].x, s[a].y, s[b].x, s[b].y, thickness, color);
		};

	L(0, 1); L(1, 3); L(3, 2); L(2, 0);
	L(4, 5); L(5, 7); L(7, 6); L(6, 4);
	L(0, 4); L(1, 5); L(2, 6); L(3, 7);
}

void Drawing::drawDirectionLine(const Vec3& pos, float yaw, float length, D3DCOLOR color)
{
	Vec3 rayEnd;
	rayEnd.x = pos.x + cosf(yaw + (M_PI / 2)) * length; // M_PI_2 = pi/2
	rayEnd.y = pos.y + sinf(yaw + (M_PI / 2)) * length;
	rayEnd.z = pos.z;

	Vec2 screenStart, screenEnd;
	if (W2S(pos, screenStart) && W2S(rayEnd, screenEnd))
	{
		drawLine(screenStart.x, screenStart.y, screenEnd.x, screenEnd.y, 2, color);
	}
}

void Drawing::updateViewMatrix()
{
	//Setup viewMatrix here
	//memcpy(&viewMatrix, playerData.viewMatrix, sizeof(viewMatrix));
}

Drawing::Drawing()
{
}

bool Drawing::worldToScreen(Vec3 pos, Vec2& screen, int windowWidth, int windowHeight)
{
	updateViewMatrix();
	//Matrix-vector Product, multiplying world(eye) coordinates by projection matrix = clipCoords
	Vec4 clipCoords;
	clipCoords.x = pos.x * viewMatrix[0] + pos.y * viewMatrix[4] + pos.z * viewMatrix[8] + viewMatrix[12];
	clipCoords.y = pos.x * viewMatrix[1] + pos.y * viewMatrix[5] + pos.z * viewMatrix[9] + viewMatrix[13];
	clipCoords.z = pos.x * viewMatrix[2] + pos.y * viewMatrix[6] + pos.z * viewMatrix[10] + viewMatrix[14];
	clipCoords.w = pos.x * viewMatrix[3] + pos.y * viewMatrix[7] + pos.z * viewMatrix[11] + viewMatrix[15];


	if (clipCoords.w < 1.0f)
		return false;

	//perspective division, dividing by clip.W = Normalized Device Coordinates
	Vec3 NDC;
	NDC.x = clipCoords.x / clipCoords.w;
	NDC.y = clipCoords.y / clipCoords.w;
	NDC.z = clipCoords.z / clipCoords.w;

	screen.x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
	screen.y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);
	return true;
}