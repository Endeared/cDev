#include "stdafx.h"
#include "proc.h"

// process ID in windows API is DWORD
DWORD GetProcId(const wchar_t* procName) {

	// getting snapshot of processes, setting process id to 0
	DWORD procId = 0;
	HANDLE hSnap = (CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0));

	// CreateToolhelp32Snapshot will return INVALID_HANDLE_VALUE if it errors, so
	// we check for that error
	if (hSnap != INVALID_HANDLE_VALUE) {

		// if no error, we set our empty procEntry variable and specify a size
		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(procEntry);

		// looping through all processes with Process32First and Process32Next by supplying hSnap,
		// and assigning the process to procEntry
		if (Process32First(hSnap, &procEntry)) {
			do {
				// if our wide char string insensitive comparison of file name / process name
				if (!_wcsicmp(procEntry.szExeFile, procName)) {
					// assign process id to procId, break
					procId = procEntry.th32ProcessID;
					break;
				}
			} while (Process32Next(hSnap, &procEntry));
		}
	}
	// close handle and return process id
	CloseHandle(hSnap);
	return procId;

}

// remember to compile in same architecture as game (x86 or x64)
uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName) {

	// grab handle snapshot of all modules for process
	uintptr_t modBaseAddr = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);

	// if CreateToolhelp32Snapshot doesn't return invalid handle, we continue
	if (hSnap != INVALID_HANDLE_VALUE) {

		// empty modEntry, initialize size
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);

		// grab first module and assign to modEntry
		if (Module32First(hSnap, &modEntry)) {
			// continue looping thru all modules while there is a next module
			do {
				// if module name matches, copy address into our modBaseAddr and break
				if (!_wcsicmp(modEntry.szModule, modName)) {
					modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
					break;
				}
			} while (Module32Next(hSnap, &modEntry));
		}
	}
	// close handle and return address
	CloseHandle(hSnap);
	return modBaseAddr;

}

// dynamic memory allocation address - takes in handle, base ptr, and vector of offsets
uintptr_t findDMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets) {

	uintptr_t addr = ptr;
	for (unsigned int i = 0; i < offsets.size(); i++) {
		ReadProcessMemory(hProc, (BYTE*)addr, &addr, sizeof(addr), 0);
		addr += offsets[i];
	}

	return addr;
}