#pragma once
#include <windows.h>
#include <string>
#include <TlHelp32.h>

struct Module
{
	uintptr_t module_base;
	DWORD module_size;
};

class Memory
{
private:
	Module	client_dll;
	Module	engine_dll;

	uintptr_t	process_id;
	HANDLE		process_handle;

public:
	Memory();
	~Memory();

	HANDLE get_process() const;

	Module get_client() const;
	Module get_engine() const;

	Module get_module(std::string module) const;
	bool get_process_handle(std::string process);

	bool initialize(std::string process);

	template<typename T>
	T read_memory(uintptr_t address);
	template<typename T>
	void write_memory(uintptr_t address, T value);
	bool read_into(const uint32_t& address, void* buffer, const size_t& size);

	DWORD pattern_scan(HANDLE hProc, DWORD base, DWORD len, BYTE* sig, const char* mask, int offset = 0);
};

extern Memory memory;

template<typename T>
inline T Memory::read_memory(uintptr_t address)
{
	T buffer;
	ReadProcessMemory(process_handle, reinterpret_cast<LPCVOID>(address), &buffer, sizeof(T), nullptr);
	return buffer;
}

template<typename T>
inline void Memory::write_memory(uintptr_t address, T value)
{
	WriteProcessMemory(process_handle, reinterpret_cast<LPVOID>(address), &value, sizeof(T), nullptr);
}

inline bool Memory::read_into(const uint32_t& address, void* buffer, const size_t& size)
{
	SIZE_T bytes_readed = 0;
	return !!ReadProcessMemory(process_handle, reinterpret_cast< const void* >(address), buffer, size, &bytes_readed);
}