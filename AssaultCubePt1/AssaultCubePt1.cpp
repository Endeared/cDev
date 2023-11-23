// includes for standard libraries + our header
#include <iostream>
#include <vector>
#include <Windows.h>
#include "proc.h"

int main()
{
    // get procid
    DWORD procId = GetProcId(L"ac_client.exe");

    // get module base addr
    uintptr_t moduleBase = GetModuleBaseAddress(procId, L"ac_client.exe");

    // get process handle
    HANDLE hProcess = 0;
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);
    
    // resolve base addr
    uintptr_t dynamicPtrBase = moduleBase + 0x10F4F4;
    std::cout << "DynamicPtrBaseAddr = " << "0x" << std::hex << dynamicPtrBase << std::endl;

    // resolve ammo ptr
    // method with multilevel offsets:
    // std::vector<unsigned int> ammoOffsets = {0x374, 0x14, 0x0};
    // uintptr_t ammoAddr = findDMAAddy(hProcess, dynamicPtrBase, ammoOffsets);
    // method with ptr from ptrscan:
    uintptr_t ammoAddr = 0x0123B468;
    std::cout << "ammoAddr = " << "0x" << std::hex << ammoAddr << std::endl;

    // read ammo value
    int ammoValue = 0;
    ReadProcessMemory(hProcess, (BYTE*)ammoAddr, &ammoValue, sizeof(ammoValue), nullptr);
    std::cout << "Current ammo = " << std::dec << ammoAddr << std::endl;

    // write to ammo value
    int newAmmo = 1000;
    WriteProcessMemory(hProcess, (BYTE*)ammoAddr, &newAmmo, sizeof(newAmmo), nullptr);

    // read ammo value again (confirm works)
    int finalAmmo = 0;
    ReadProcessMemory(hProcess, (BYTE*)ammoAddr, &finalAmmo, sizeof(finalAmmo), nullptr);
    std::cout << "Final ammo = " << std::dec << finalAmmo << std::endl;


    std::cout << std::endl;
    // same steps above, but for health
    uintptr_t healthAddr = 0x0123B420;
    std::cout << "healthAddr = " << "0x" << std::hex << healthAddr << std::endl;

    int healthValue = 0;
    ReadProcessMemory(hProcess, (BYTE*)healthAddr, &healthValue, sizeof(healthValue), nullptr);
    std::cout << "Current health = " << std::dec << ammoAddr << std::endl;

    int newHealth = 9999;
    WriteProcessMemory(hProcess, (BYTE*)healthAddr, &newHealth, sizeof(newHealth), nullptr);

    int finalHealth = 0;
    ReadProcessMemory(hProcess, (BYTE*)healthAddr, &finalHealth, sizeof(finalHealth), nullptr);
    std::cout << "Final health = " << std::dec << finalHealth << std::endl;
    


    return 0;
}

