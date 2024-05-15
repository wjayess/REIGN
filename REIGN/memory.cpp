// Windows
#include <Windows.h>
#include <string_view>
#include <TlHelp32.h>

#include "memory.h"

// Constructor that finds the process id and opens a handle
Memory::Memory(std::string_view processName) noexcept
{
	PROCESSENTRY32 entry = {};
	entry.dwSize = sizeof(PROCESSENTRY32);

	const HANDLE snapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	while (Process32Next(snapShot, &entry))
	{
		if (!processName.compare(entry.szExeFile))
		{
			processId = entry.th32ProcessID;
			processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);

			break;
		}
	}

	// Free handle
	if (snapShot)
		CloseHandle(snapShot);
}

// Destructor that frees opened handle
Memory::~Memory()
{
	if (processHandle)
		CloseHandle(processHandle);
}

// Returns the base address of a module by name
std::uintptr_t Memory::GetModuleAddress(std::string_view moduleName) const noexcept
{
	MODULEENTRY32 entry = {};
	entry.dwSize = sizeof(MODULEENTRY32);

	const HANDLE snapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processId);

	std::uintptr_t result = 0;

	while (Module32Next(snapShot, &entry))
	{
		if (!moduleName.compare(entry.szModule))
		{
			result = reinterpret_cast<std::uintptr_t>(entry.modBaseAddr);
			break;
		}
	}

	if (snapShot)
		CloseHandle(snapShot);

	return result;
}