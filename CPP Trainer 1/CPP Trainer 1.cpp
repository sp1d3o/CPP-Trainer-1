﻿// CPP Trainer 1.cpp : Tento soubor obsahuje funkci main. Provádění programu se tam zahajuje a ukončuje.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <Windows.h>
#include "proc.h"

int main()
{
    //Get ProcId of the current process
    DWORD procId = GetProcId(L"ac_client.exe");

    //Get Module Base Address
    uintptr_t moduleBase = GetModuleBaseAddress(procId, L"ac_client.exe");

    //Get Handle to Process
    HANDLE hProcess = 0;
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

    //Resolve Base Address
    uintptr_t dynamicPtrBaseAddr = moduleBase + 0x10F4F4;

    std::cout << "Dynamic base address = " << "0x" << std::hex << dynamicPtrBaseAddr << std::endl;

    //Resolve our ammo pointer chain
    std::vector<unsigned int> ammoOffsets = { 0x374, 0x14, 0x0 };
    uintptr_t ammoAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, ammoOffsets);

    std::cout << "Ammo Address = " << "0x" << std::hex << ammoAddr << std::endl;

    //Read Ammo Value
    int ammoValue = 0;
    ReadProcessMemory(hProcess, (BYTE*)ammoAddr, &ammoValue, sizeof(ammoValue), nullptr);
    
    std::cout << "Current ammo value = " << std::dec << ammoValue << std::endl;
    
    //Write to it
    int newAmmo = 1337;
    WriteProcessMemory(hProcess, (BYTE*)ammoAddr, &newAmmo, sizeof(newAmmo), nullptr);

    //Read Ammo Value again to check
    ReadProcessMemory(hProcess, (BYTE*)ammoAddr, &ammoValue, sizeof(ammoValue), nullptr);
    std::cout << "New ammo value = " << std::dec << ammoValue << std::endl;
}

