#pragma once
#include "include.h"

class Hooking {
public:
	static HRESULT APIENTRY hookedReset(LPDIRECT3DDEVICE9 o_pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
	static HRESULT APIENTRY hookedPresent(LPDIRECT3DDEVICE9 o_pDevice, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);

	static void implementHooking();
	static void unHook();
	static void freeGateway();
};

