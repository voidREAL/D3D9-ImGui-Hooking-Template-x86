#include <Windows.h>
#include <iostream>

#include "../framework/include/mem.h"
#include "../include/dx.h"
#include "../include/rendercore.h"
#include "../include/hackrender.h"
#include "../include/drawing.h"
#include "../include/hooking.h"

DWORD WINAPI HackThread(HMODULE hModule) {
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);	
	MemoryInternal mem;

	Hooking::ImplementHooking();

	//Keep DLL dont eject
	while (true) {
		if (!render.state.showMenu) {
			if (GetAsyncKeyState(VK_END)) {
				break;
			}
		}
	}

	//Unhook and cleanup
	{		
		Hooking::UnHook();
		render.cleanup();

		//Set Original WndProc to game
		render.setWndProcToOriginal();

		//Đợi 1 giây trước khi VirtualFree vì nếu có luồng đang chạy ở trong gateway thì rất dễ xung đột và crash, đợi 1 giây cho luồng chạy xong đã
		Sleep(1000);

		FreeConsole();
		if (f) {
			fclose(f);
		}

		Hooking::FreeGateway();

		FreeLibraryAndExitThread(hModule, 0);
		return 0;
	}
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
		HANDLE thread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, nullptr);
		if (thread) {
			CloseHandle(thread);
		}
    }
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

