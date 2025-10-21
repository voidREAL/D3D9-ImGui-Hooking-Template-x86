#pragma once

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_win32.h"
#include "../imgui/imgui_impl_dx9.h"
#include "rendercore.h"

struct ESPState {
	bool isSnaplineOn;
	bool isBox2DOn;
	bool isStatusTextOn;
	bool isBoneESPOn;
	bool isBox3DOn;
	bool isHeadLineESPOn;
	bool isPlayerNameTextOn;
	bool isCurrentWeaponHandleOn;
};

struct ESPColors {
	ImVec4 snapLineColor = { 0,0,0,255 };
	ImVec4 box2DColor = { 0,0,0,255 };
	ImVec4 statusTextColor = { 0,0,0,255 };
	ImVec4 boneESPColor = { 0,0,0,255 };
	ImVec4 box3DColor = { 0,0,0,255 };
	ImVec4 headLineESPColor = { 0,0,0,255 };
	ImVec4 playerNameColor = { 0,0,0,255 };
	ImVec4 currentWeaponHandleColor = { 0,0,0,255 };
};

struct AimbotState {
	bool isSilentAimOn;
};

struct MiscState {
	bool isNoClipOn;
	bool isSpeedHackOn;
	bool isGravityHackOn;
	bool isFullStatsOn;
	bool isTeleportHackOn;
	bool isSaveCoordOn;
	bool isGodModeHackOn;
	bool isGodWeaponOn;
};

class HackRender {
public:
	struct State {
		bool isEspOn;
		ESPState espState;

		bool isAimbotOn;
		AimbotState aimbotState;

		bool isMiscOn;
		MiscState miscState;
	} state;

	struct Colors {
		ESPColors espColors;
	} colors;

	static void render();
	void espRender();
	void aimbotRender();
	void miscRender();
};

extern HackRender hackRender;