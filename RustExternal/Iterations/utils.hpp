#pragma once
#include <Windows.h>
#include <TlHelp32.h>

namespace util
{
	inline unsigned get_pid(const char* proc_name)
	{
		PROCESSENTRY32 proc_info;
		proc_info.dwSize = sizeof(proc_info);

		const auto proc_snapshot =
			CreateToolhelp32Snapshot(
				TH32CS_SNAPPROCESS,
				NULL
			);

		if (proc_snapshot == INVALID_HANDLE_VALUE)
			return NULL;

		Process32First(proc_snapshot, &proc_info);
		if (!strcmp(proc_info.szExeFile, proc_name))
		{
			CloseHandle(proc_snapshot);
			return proc_info.th32ProcessID;
		}

		while (Process32Next(proc_snapshot, &proc_info))
		{
			if (!strcmp(proc_info.szExeFile, proc_name))
			{
				CloseHandle(proc_snapshot);
				return proc_info.th32ProcessID;
			}
		}

		CloseHandle(proc_snapshot);
		return {};
	}

	inline uintptr_t dwGetModuleBaseAddress(unsigned int pid, const wchar_t* szName)
	{
		if (!szName)
			return false;

		MODULEENTRY32W ME32;
		HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);

		if (!hSnap)
			return false;

		ME32.dwSize = { sizeof(MODULEENTRY32W) };

		if (Module32FirstW(hSnap, &ME32))
		{
			do
			{
				if (_wcsicmp(ME32.szModule, szName) == 0)
				{
					uintptr_t mBase = reinterpret_cast<uintptr_t>(ME32.modBaseAddr);
					CloseHandle(hSnap);
					return mBase;
				}
			} while (Module32NextW(hSnap, &ME32));
		}

		CloseHandle(hSnap);
		return false;
	}
}