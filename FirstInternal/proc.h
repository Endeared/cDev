#pragma once
#include <vector>
#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>
//https://guidedhacking.com

DWORD GetProcId(const wchar_t* procName);

uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName);

uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets);