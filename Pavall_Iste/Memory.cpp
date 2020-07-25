#include "Memory.hpp"

Memory memory;

Memory::Memory()
{
	process_id = NULL;
	process_handle = nullptr;
}

Memory::~Memory()
{
	CloseHandle(process_handle);
}

HANDLE Memory::get_process() const
{
	return process_handle;
}

Module Memory::get_client() const
{
	return client_dll;
}

Module Memory::get_engine() const
{
	return engine_dll;
}

Module Memory::get_module(std::string module) const
{
	HANDLE local_process_handle = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, process_id);

	if (local_process_handle == INVALID_HANDLE_VALUE)
		return Module();

	MODULEENTRY32 module_entry;
	module_entry.dwSize = sizeof(MODULEENTRY32);

	if (Module32First(local_process_handle, &module_entry))
	{
		while (Module32Next(local_process_handle, &module_entry))
		{
			if (std::string(module_entry.szModule) == module)
			{
				CloseHandle(local_process_handle);

				Module return_module;
				return_module.module_base = reinterpret_cast<uintptr_t>(module_entry.modBaseAddr);
				return_module.module_size = module_entry.modBaseSize;
				return return_module;
			}
		}
	}
	return Module();
}

bool Memory::get_process_handle(std::string process)
{
	const auto local_process_handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (local_process_handle == INVALID_HANDLE_VALUE)
		return false;

	PROCESSENTRY32 process_entry;
	process_entry.dwSize = sizeof(PROCESSENTRY32);

	if (Process32First(local_process_handle, &process_entry))
	{
		while (Process32Next(local_process_handle, &process_entry))
		{
			if (std::string(process_entry.szExeFile).find(process) != std::string::npos)
			{
				process_id = process_entry.th32ProcessID;
				process_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, process_id);
				CloseHandle(local_process_handle);
				return true;
			}
		}
	}

	return false;
}

bool Memory::initialize(std::string process)
{
	if (process.length() <= 0) return false;

	if (!get_process_handle(process)) return false;

	client_dll = get_module("client.dll");
	engine_dll = get_module("engine.dll");

	return(client_dll.module_base && engine_dll.module_base);
}

DWORD Memory::pattern_scan(HANDLE hProc, DWORD base, DWORD len, BYTE* sig, const char* mask, int offset)
{
	BYTE* buf = (BYTE*)VirtualAlloc(0, len, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (ReadProcessMemory(hProc, (LPCVOID)base, buf, len, NULL) == false)
	{
		char buf2[64];
		return NULL;
	}
	for (int i = 0; i <= (len - strlen(mask)) + 1; i++)
	{
		if ((buf[i] == sig[0] && mask[0] == 'x') || (mask[0] == '?'))
		{
			for (int x = 0;; x++)
			{
				if (mask[x] == 'x')
				{
					if (buf[i + x] == sig[x])
						continue;
					else
						break;
				}
				else if (mask[x] == 0x00) {
					return (DWORD)(base + i + offset);
				}
			}
		}
	}
	return NULL;
}