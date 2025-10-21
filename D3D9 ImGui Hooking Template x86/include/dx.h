#pragma once
#include "include.h"

using _Present = HRESULT(APIENTRY*)(LPDIRECT3DDEVICE9 pDevice, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);
using _Reset = HRESULT(APIENTRY*)(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);

class DXD3D9 {
public:
	void* d3d9Vtable[119];

	bool GetD3D9DeviceVTable(void** pTable, size_t size);
	bool GetDInputDeviceVTable(void** table, size_t size);
	HWND GetProcessWindow();

	HWND window;

	int windowHeight, windowWidth;

	LPDIRECT3DDEVICE9 pDevice;
	ID3DXLine* lineL;
	ID3DXFont* font;

	//function pointer
	_Present present;
	_Reset resetFunc;

	DXD3D9();
};

extern DXD3D9 d3d9;







