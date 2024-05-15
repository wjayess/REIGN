#pragma once

class Memory
{
private:
	int processId = 0;
	void* processHandle = nullptr;

public:
	Memory(std::string_view processName) noexcept;
	~Memory();

	std::uintptr_t GetModuleAddress(std::string_view moduleName) const noexcept;

	// Read process memory
	template <typename T>
	const T Read(const std::uintptr_t address) const noexcept
	{
		T value = {};

		ReadProcessMemory(processHandle, reinterpret_cast<LPVOID>(address), &value, sizeof(T), NULL);

		return value;
	}

	bool ReadRawMem(uintptr_t addr, void* buffer, size_t size)
	{
		SIZE_T bytesRead;

		if (ReadProcessMemory(processHandle, reinterpret_cast<LPCVOID>(addr), buffer, size, &bytesRead))
		{
			return bytesRead == size;
		}

		return false;
	}

	// Write process memory
	//template <typename T>
	//void Write(const int address, const T& value) const noexcept
	//{
	//	WriteProcessMemory(processHandle, reinterpret_cast<void*>(address), &value, sizeof(T), NULL);
	//}
};