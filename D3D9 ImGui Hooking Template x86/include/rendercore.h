#pragma once
#include <Windows.h>

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_win32.h"
#include "../imgui/imgui_impl_dx9.h"

#include "include.h"
class Render {
public:
	struct State {
		bool isImGuiInit;

		bool showMenu;
		bool showDemoWindow;
	} state;

	bool init(HWND window, LPDIRECT3DDEVICE9 device);
	bool loop(void(*renderCallback)());
	bool cleanup();
	HRESULT handleLostDevice(void* resetFuncPointer, LPDIRECT3DDEVICE9 device, D3DPRESENT_PARAMETERS* pPresentationParameters);

	static LRESULT WINAPI WndProcHook(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static bool handleImGuiInput(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static LONG_PTR originalWndProc;
	static HWND window;

	bool setHookWndProc(HWND handleWindow, LONG_PTR wndProc = (LONG_PTR)WndProcHook);
	bool setWndProcToOriginal();
};

extern Render render;