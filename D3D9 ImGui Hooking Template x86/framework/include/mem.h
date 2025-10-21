#pragma once
#include <Windows.h>
#include <vector>
#include <iostream>

class MemoryInternal
{
public:
	MemoryInternal();
	~MemoryInternal();

	uintptr_t findDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets);
	bool patch(BYTE* dst, BYTE* src, size_t size);
	bool nop(BYTE* dst, size_t size);
	bool hook(BYTE* dst, BYTE* src, size_t size);
	BYTE* trampolineHook(BYTE* dst, BYTE* src, size_t size);
	BYTE* hookMidFunction(BYTE* dst, BYTE* src, size_t size);
	bool unHook(BYTE* dst, BYTE* src, size_t size);
};

