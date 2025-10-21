#include "../include/hackrender.h"

HackRender hackRender;

void HackRender::render()
{
	ImGuiWindowFlags windowsFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
	ImGui::SetNextWindowSize(ImVec2(600, 400));
	if (ImGui::Begin("First window", 0, windowsFlags)) {

		ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
		if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
		{
			if (ImGui::BeginTabItem("ESP"))
			{
				hackRender.espRender();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Aimbot"))
			{
				hackRender.aimbotRender();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Misc"))
			{
				hackRender.miscRender();
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::End();
	}

	//ImGui::ShowDemoWindow((bool*)1);
}

void HackRender::espRender() {
	if (ImGui::Checkbox("Enable", &state.isEspOn));
	if (state.isEspOn) {
		ImGuiColorEditFlags colorsEditFlags = ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_NoInputs;
		ImGui::Checkbox("SnapLine", &state.espState.isSnaplineOn); ImGui::SameLine(); ImGui::ColorEdit4("1", (float*)&colors.espColors.snapLineColor, colorsEditFlags);
		ImGui::Checkbox("Box 2D", &state.espState.isBox2DOn); ImGui::SameLine(); ImGui::ColorEdit4("2", (float*)&colors.espColors.box2DColor, colorsEditFlags);
		ImGui::Checkbox("Status Text", &state.espState.isStatusTextOn); ImGui::SameLine(); ImGui::ColorEdit4("3", (float*)&colors.espColors.statusTextColor, colorsEditFlags);
		ImGui::Checkbox("Bone", &state.espState.isBoneESPOn); ImGui::SameLine(); ImGui::ColorEdit4("4", (float*)&colors.espColors.boneESPColor, colorsEditFlags);
		ImGui::Checkbox("Box 3D", &state.espState.isBox3DOn); ImGui::SameLine(); ImGui::ColorEdit4("5", (float*)&colors.espColors.box3DColor, colorsEditFlags);
		ImGui::Checkbox("Head line", &state.espState.isHeadLineESPOn); ImGui::SameLine(); ImGui::ColorEdit4("6", (float*)&colors.espColors.headLineESPColor, colorsEditFlags);
		ImGui::Checkbox("Player Name", &state.espState.isPlayerNameTextOn); ImGui::SameLine(); ImGui::ColorEdit4("7", (float*)&colors.espColors.playerNameColor, colorsEditFlags);
		ImGui::Checkbox("Current Weapon Player Handle", &state.espState.isCurrentWeaponHandleOn); ImGui::SameLine(); ImGui::ColorEdit4("8", (float*)&colors.espColors.currentWeaponHandleColor, colorsEditFlags);
	}
}

void HackRender::aimbotRender() {
	ImGui::Checkbox("Enable", &state.isAimbotOn);
	if (state.isAimbotOn) {
		ImGui::Checkbox("Silent Aim", &state.aimbotState.isSilentAimOn);
	}
}

void HackRender::miscRender() {
	ImGui::Checkbox("Enable", &state.isMiscOn);
	if (state.isMiscOn) {
		ImGui::Checkbox("No clip", &state.miscState.isNoClipOn);
		ImGui::Checkbox("Speed Hack", &state.miscState.isSpeedHackOn);
		ImGui::Checkbox("Gravity Hack", &state.miscState.isGravityHackOn);
		ImGui::Checkbox("Full Stats", &state.miscState.isFullStatsOn);
		ImGui::Checkbox("Teleport", &state.miscState.isTeleportHackOn);
		ImGui::Checkbox("Save Coords", &state.miscState.isSaveCoordOn);
		ImGui::Checkbox("God Mode", &state.miscState.isGodModeHackOn);
		ImGui::Checkbox("God Weapon", &state.miscState.isGodWeaponOn);
	}
}
