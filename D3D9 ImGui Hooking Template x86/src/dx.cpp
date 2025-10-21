#include "../include/dx.h"

DXD3D9 d3d9;

BOOL CALLBACK enumWind(HWND hwnd, LPARAM lParam) {
	DWORD procId;
	GetWindowThreadProcessId(hwnd, &procId);
	if (procId == GetCurrentProcessId()) {
		d3d9.window = hwnd;
		return FALSE;
	}
	return TRUE;
}

HWND DXD3D9::getProcessWindow() {
	window = NULL;

	EnumWindows(enumWind, NULL);

	RECT size;

	if(!window) {
		return NULL;
	}

	GetClientRect(window, &size);
	windowHeight = size.bottom - size.top;
	windowWidth = size.right - size.left;

	return window;
}

bool DXD3D9::getD3D9DeviceVTable(void** pTable, size_t size) {
	if (!pTable) {
		return false;
	}

	IDirect3D9* pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if(!pD3D) {
		return false;
	}

	IDirect3DDevice9* dummyDevice = nullptr;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = false;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = getProcessWindow();

	HRESULT dummyDevCreated = pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		d3dpp.hDeviceWindow,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&dummyDevice
	);

	if (dummyDevCreated != S_OK) {
		d3dpp.Windowed = !d3dpp.Windowed;
		dummyDevCreated = pD3D->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			d3dpp.hDeviceWindow,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&dummyDevice
		);

		if (dummyDevCreated != S_OK) {
			pD3D->Release();
			return false;
		}
	}

	memcpy(pTable, *(void***)(dummyDevice), size);
	dummyDevice->Release();
	pD3D->Release();
	return true;

}

//Get vtable of DirectInputDevice8, for hooking GetDeviceState, like vtable of Direct3D9Device
bool DXD3D9::getDInputDeviceVTable(void** table, size_t size) {
	IDirectInput8A* pDI = nullptr;
	IDirectInputDevice8A* pMouse = nullptr;

	DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8A, (void**)&pDI, NULL);
	
	pDI->CreateDevice(GUID_SysMouse, &pMouse, NULL);
	void** vtbl = *(void***)pMouse;
	memcpy(table, vtbl, size);

	pMouse->Release();
	pDI->Release();

	return true;
}

DXD3D9::DXD3D9()
{
	window = nullptr;
	windowHeight = 0;
	windowWidth = 0;
	pDevice = nullptr;
	lineL = nullptr;
	font = nullptr;

	present = nullptr;
	resetFunc = nullptr;
	isResourceInit = false;
}
