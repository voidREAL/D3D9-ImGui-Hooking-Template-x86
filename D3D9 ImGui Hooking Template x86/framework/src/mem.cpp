#include "../include/mem.h"

MemoryInternal::MemoryInternal()
{
}

MemoryInternal::~MemoryInternal()
{
}

uintptr_t MemoryInternal::findDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets)
{
	uintptr_t addr = ptr;
	for (unsigned int i = 0; i < offsets.size(); ++i)
	{
		addr = *(uintptr_t*)addr;
		addr += offsets[i];
	}
	return addr;
}

bool MemoryInternal::patch(BYTE* dst, BYTE* src, size_t size)
{
	DWORD oldProtect;
	if (!VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldProtect)) {
		std::cout << "VirtualProtectEx failed: " << GetLastError() << std::endl;
		return false;
	}
	memcpy(dst, src, size);
	VirtualProtect(dst, size, oldProtect, &oldProtect);
	return true;
}

bool MemoryInternal::nop(BYTE* dst, size_t size)
{
	DWORD oldProtect;
	if (!VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldProtect)) {
		std::cout << "VirtualProtectEx failed: " << GetLastError() << std::endl;
		return false;
	}
	memset(dst, 0x90, size);
	VirtualProtect(dst, size, oldProtect, &oldProtect);
	return true;
}

bool MemoryInternal::hook(BYTE* dst, BYTE* src, size_t size)
{
	if (size < 5) {
		return false;
	}
	DWORD oldProtect;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldProtect);

	memset(dst, 0x90, size);

	intptr_t relativeAddress = ((intptr_t)src - (intptr_t)dst) - 0x5;

	*(BYTE*)(dst) = 0xE9;

	*(intptr_t*)(dst + 1) = relativeAddress;

	VirtualProtect(dst, size, oldProtect, &oldProtect);

	return true;
}

BYTE* MemoryInternal::trampolineHook(BYTE* dst, BYTE* src, size_t size)
{
	if (size < 5) {
		return 0;
	}

	VOID* gateway = VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	memcpy_s(gateway, size, dst, size);

	intptr_t relativeAddress = (((intptr_t)dst - (intptr_t)gateway) - 5);

	memset((BYTE*)((intptr_t)gateway + size), 0xE9, 1);

	*(intptr_t*)((intptr_t)gateway + size + 1) = relativeAddress;

	hook(dst, src, size);

	return (BYTE*)gateway;
}

BYTE* MemoryInternal::hookMidFunction(BYTE* dst, BYTE* src, size_t size)
{
	if (size < 5) {
		return 0;
	}

	DWORD currentProtectDst, currentProtectSrc;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &currentProtectDst);
	BYTE* gateway = (BYTE*)VirtualAlloc(nullptr, size + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (gateway) {
		memset(gateway, 0x90, 5);
		memset(gateway, 0xE8, 1);
		intptr_t relativeAddressToGateway = (intptr_t)src - (intptr_t)gateway - 5;
		*(intptr_t*)(gateway + 0x1) = relativeAddressToGateway;
		memcpy(gateway + 5, dst, size);
		intptr_t relativeAddressBack = ((intptr_t)dst - (intptr_t)gateway) - size - 4;
		memset(gateway + size + 5, 0xE9, 1);
		*(intptr_t*)(gateway + size + 6) = relativeAddressBack;
	}

	memset(dst, 0x90, size);

	intptr_t relativeAddress = ((intptr_t)gateway - (intptr_t)dst - 5);

	memset(dst, 0xE9, 1);
	*(intptr_t*)(dst + 0x1) = relativeAddress;

	VirtualProtect(dst, size, currentProtectDst, &currentProtectDst);
	return gateway;
}

bool MemoryInternal::unHook(BYTE* dst, BYTE* src, size_t size)
{
	return patch(dst, dst, size);
}

