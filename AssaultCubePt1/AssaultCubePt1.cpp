// includes for standard libraries + our header
#include "stdafx.h"
#include <iostream>
#include <vector>
#include <Windows.h>
#include "proc.h"
#include "mem.h"

int main() {
    // defining vars
    HANDLE hProcess = 0;
    uintptr_t moduleBase = 0, localPlayerPtr = 0, healthAddr = 0;
    bool bHealth = false, bAmmo = false, bRecoil = false;
    const int newValue = 9999;
    
    // grabbing pid
    DWORD procId = GetProcId(L"ac_client.exe");

    // if pid is found, open process, get module addr, assign dynamic ptr addr,
    // then locate health addr with finddmaaddy
    if (procId) {
        hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

        moduleBase = GetModuleBaseAddress(procId, L"ac_client.exe");

        uintptr_t dynamicPtrBase = moduleBase + 0x10F4F4;

        healthAddr = findDMAAddy(hProcess, dynamicPtrBase, { 0xF8 });
    }
    // otherwise, print generic message and exit
    else {
        std::cout << "Process couldn't be found - press enter to exit.\n";
        getchar();
        return 0;
    }

    // DWORD exit code
    DWORD dwExit = 0;

    // everytime loop runs, calls GetExitCodeProcess and returns nonzero num or 0 if function fails
    // (checks if process is still running)
    while (GetExitCodeProcess(hProcess, &dwExit) && dwExit == STILL_ACTIVE) {

        // hotkey options. 1 = toggles health
        if (GetAsyncKeyState(VK_F1) & 1) {
            bHealth = !bHealth;
        }

        // 2 = toggles ammo
        if (GetAsyncKeyState(VK_F2) & 1) {
            bAmmo = !bAmmo;

            // if ammo is enabled, patch addr with inc [esi] (FF 06)
            if (bAmmo) {
                mem::PatchEx((BYTE*)(moduleBase + 0x637E9), (BYTE*)"\xFF\x06", 2, hProcess);
            }
            // otherwise, ammo is disabled, so patch addr with dec [esi] (FF 0E);
            else {
                mem::PatchEx((BYTE*)(moduleBase + 0x637E9), (BYTE*)"\xFF\x0E", 2, hProcess);
            }
        } 

        // 3 = toggles recoil
        if (GetAsyncKeyState(VK_F3) & 1) {
            bRecoil = !bRecoil;

            // if recoil enabled, patch recoil addr with nop instructions (removes recoil)
            if (bRecoil) {
                mem::NopEx((BYTE*)(moduleBase + 0x63786), 10, hProcess);
            }
            // otherwise, patch recoil addr with original array of byte instructions (50 8D 4C 24 1C 51 8B CE FF D2)
            else {
                mem::PatchEx((BYTE*)(moduleBase + 0x63786), (BYTE*)"\x50\x8D\x4C\x24\x1C\x51\x8B\xCE\xFF\xD2", 10, hProcess);
            }
        }

        // insert = closes menu / trainer
        if (GetAsyncKeyState(VK_INSERT) & 1) {
            return 0;
        }

        // continuous write
        if (bHealth) {
            // patch health address with new value every 10ms (imitate godmode)
            mem::PatchEx((BYTE*)healthAddr, (BYTE*)&newValue, sizeof(newValue), hProcess);
        }

        Sleep(10);
    }

    std::cout << "Process not found - press enter to exit.\n";
    getchar();
    return 0;
}

int old()
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

