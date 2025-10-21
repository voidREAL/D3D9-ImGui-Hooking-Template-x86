#include "../include/rendercore.h"
#include "../include/dx.h"
#include "../framework/include/mem.h"

Render render;
LONG_PTR Render::originalWndProc = NULL;
HWND Render::window = NULL;

void* diVTable[20];

using _SetCursorPos = BOOL(WINAPI*)(int x, int y);
using _ClipCursor = BOOL(WINAPI*)(const RECT* lpRect);
using _GetDeviceState = HRESULT(APIENTRY*)(LPVOID, DWORD, LPVOID);

_SetCursorPos setCursorPos = nullptr;
_ClipCursor clipCursor = nullptr;
_GetDeviceState getDeviceState = nullptr;

void* addrSetCursorPos = nullptr;
void* addrClipCursor = nullptr;

BOOL WINAPI hkSetCursorPos(int X, int Y);
BOOL WINAPI hkClipCursor(const RECT* lpRect);
HRESULT __stdcall hkGetDeviceState(LPVOID device, DWORD cbData, LPVOID lpvData);

bool Render::init(HWND window, LPDIRECT3DDEVICE9 device)
{
	//Init state here
	{
		render.state.showMenu = false;
		render.state.isImGuiInit = false;
		render.state.showDemoWindow = false;
	}

	//Hooking important function for ImGui Working
	{
		addrSetCursorPos = (void*)GetProcAddress(GetModuleHandleA("user32.dll"), "SetCursorPos");
		addrClipCursor = (void*)GetProcAddress(GetModuleHandleA("user32.dll"), "ClipCursor");

		MemoryInternal mem;

		if (addrSetCursorPos) {
			setCursorPos = (_SetCursorPos)mem.trampolineHook((BYTE*)addrSetCursorPos, (BYTE*)hkSetCursorPos, 6);
		}
		if (addrClipCursor) {
			clipCursor = (_ClipCursor)mem.trampolineHook((BYTE*)addrClipCursor, (BYTE*)hkClipCursor, 6);
		}

		if (d3d9.GetDInputDeviceVTable(diVTable, sizeof(diVTable))) {
			getDeviceState = (_GetDeviceState)mem.trampolineHook((BYTE*)diVTable[9], (BYTE*)hkGetDeviceState, 5);
		}
	}

	ImGui_ImplWin32_EnableDpiAwareness();
	float main_scale = ImGui_ImplWin32_GetDpiScaleForMonitor(::MonitorFromPoint(POINT{ 0, 0 }, MONITOR_DEFAULTTOPRIMARY));
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui::StyleColorsLight();

	ImGuiStyle& style = ImGui::GetStyle();
	style.ScaleAllSizes(main_scale);
	style.FontScaleDpi = main_scale;

	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(device);

	return true;
}

bool Render::loop(void(*renderCallback)())
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	renderCallback();

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	return true;
}

bool Render::cleanup()
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	MemoryInternal mem;
	mem.patch((BYTE*)addrSetCursorPos, (BYTE*)setCursorPos, 6);
	mem.patch((BYTE*)addrClipCursor, (BYTE*)clipCursor, 6);
	mem.patch((BYTE*)diVTable[9], (BYTE*)getDeviceState, 5);
	
	Sleep(2000);

	const size_t jmpSize = 5;

	if (getDeviceState)	VirtualFree(getDeviceState, 5 + jmpSize, MEM_FREE);
	if (clipCursor) VirtualFree(clipCursor, 6 + jmpSize, MEM_FREE);
	if (setCursorPos) VirtualFree(setCursorPos, 6 + jmpSize, MEM_FREE);

	return true;
}
HRESULT Render::handleLostDevice(void* resetFuncPointer, LPDIRECT3DDEVICE9 device, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	using _Reset = HRESULT(APIENTRY*)(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
	_Reset reset = (_Reset)(resetFuncPointer);

	ImGui_ImplDX9_InvalidateDeviceObjects();
	HRESULT hr = reset(device, pPresentationParameters);
	ImGui_ImplDX9_CreateDeviceObjects();

	return hr;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT __stdcall Render::WndProcHook(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (Render::handleImGuiInput(hWnd, uMsg, wParam, lParam)) {
		return true;
	}

	return CallWindowProc((WNDPROC)originalWndProc, hWnd, uMsg, wParam, lParam);
}

bool Render::handleImGuiInput(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (!render.state.isImGuiInit) {
		return CallWindowProc((WNDPROC)originalWndProc, hWnd, uMsg, wParam, lParam);
	}

	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam)) {
		return true;
	}

	ImGuiIO& io = ImGui::GetIO();
	if (render.state.showMenu && (io.WantCaptureMouse || io.WantCaptureKeyboard))
		return true;
}

bool Render::setHookWndProc(HWND handleWindow, LONG_PTR wndProc)
{
	if (!handleWindow) {
		return false;
	}
	window = handleWindow;
	originalWndProc = SetWindowLongPtr(window, GWLP_WNDPROC, wndProc);
	return true;
}

bool Render::setWndProcToOriginal()
{
	if (window && originalWndProc) {
		SetWindowLongPtr(window, GWLP_WNDPROC, originalWndProc);
		return true;
	}
	else {
		return false;
	}
}

BOOL WINAPI hkSetCursorPos(int X, int Y)
{
	if (render.state.showMenu) {
		return TRUE;
	}

	return setCursorPos(X, Y);
}

//Cho chuot di chuyen tu do ra khoi cua so
BOOL WINAPI hkClipCursor(const RECT* lpRect)
{
	if (render.state.showMenu) {
		return TRUE;
	}
	return clipCursor(lpRect);
}

//Neu imgui duoc bat, khong cho chuot tuong tac voi game
HRESULT __stdcall hkGetDeviceState(LPVOID device, DWORD cbData, LPVOID lpvData) {
	HRESULT hr = getDeviceState(device, cbData, lpvData);

	if (SUCCEEDED(hr) && render.state.showMenu && (cbData == sizeof(DIMOUSESTATE2))) {
		memset(lpvData, 0, cbData);
	}
	return hr;
}