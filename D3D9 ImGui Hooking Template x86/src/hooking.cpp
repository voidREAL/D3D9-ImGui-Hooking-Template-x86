#include "../include/hooking.h"
#include "../include/dx.h"
#include "../include/rendercore.h"
#include "../include/hackrender.h"
#include "../framework/include/mem.h"

HRESULT APIENTRY Hooking::hookedReset(LPDIRECT3DDEVICE9 o_pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	//Check device lost, and re-alloc resources
	HRESULT hr = render.handleLostDevice((void*)d3d9.resetFunc, o_pDevice, pPresentationParameters);

	return hr;
}

HRESULT APIENTRY Hooking::hookedPresent(LPDIRECT3DDEVICE9 o_pDevice, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)
{
	if (!d3d9.pDevice) {
		d3d9.pDevice = o_pDevice;
	}

	//Init backend of imgui
	if (!render.state.isImGuiInit) {
		D3DDEVICE_CREATION_PARAMETERS params;
		d3d9.pDevice->GetCreationParameters(&params);

		d3d9.window = params.hFocusWindow;
		render.init(d3d9.window, d3d9.pDevice);
		render.state.isImGuiInit = true;
	}

	//Check ImGui isShowing
	if (GetAsyncKeyState(VK_INSERT) & 1) {
		render.state.showMenu = !render.state.showMenu;

		//If imGui show, change window proceduce pointer to hookWndProc
		if (render.state.showMenu) {
			render.setHookWndProc(d3d9.window);
		}
		//If ImGui disable, change window proceduce to original
		else {
			render.setWndProcToOriginal();
		}
	}

	//Main render <= Draw here
	if (render.state.showMenu) {
		render.loop(hackRender.render);

		if (hackRender.state.isEspOn) {
		}

		if (hackRender.state.isAimbotOn) {
		}

		if (hackRender.state.isMiscOn) {
		}
	}

	return d3d9.present(o_pDevice, NULL, NULL, NULL, NULL);
}

void Hooking::ImplementHooking()
{
	MemoryInternal mem;

	if (d3d9.GetD3D9DeviceVTable(d3d9.d3d9Vtable, sizeof(d3d9.d3d9Vtable))) {
		d3d9.present = (_Present)mem.trampolineHook((BYTE*)d3d9.d3d9Vtable[17], (BYTE*)hookedPresent, 5);
		d3d9.resetFunc = (_Reset)mem.trampolineHook((BYTE*)d3d9.d3d9Vtable[16], (BYTE*)hookedReset, 5);
	}
}

void Hooking::UnHook()
{
	MemoryInternal mem;

	mem.patch((BYTE*)d3d9.d3d9Vtable[17], (BYTE*)d3d9.present, 5);
	mem.patch((BYTE*)d3d9.d3d9Vtable[16], (BYTE*)d3d9.resetFunc, 5);
}

void Hooking::FreeGateway()
{
	const size_t jmpSize = 5;
	if (d3d9.present) VirtualFree(d3d9.present, 5 + jmpSize, MEM_FREE);
	if (d3d9.resetFunc) VirtualFree(d3d9.resetFunc, 5 + jmpSize, MEM_FREE);
}
