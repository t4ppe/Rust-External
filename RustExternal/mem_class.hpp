#pragma once
#pragma once
#include <windows.h>
#include <random>
#include <memory>
#include <array>
#include <string_view>
#include <TlHelp32.h>
#include "mem_structs.hpp"

class memory_mgr
{
private:
	HKEY _registry_handle = nullptr;
	std::string _registry_key{ };

	std::uintptr_t _process_id = 0;
	std::uintptr_t _local_process_id = 0;
public:
	memory_mgr(const std::string_view reg_key, const std::string_view process)
	{
		const auto status = RegOpenKeyExA(HKEY_CURRENT_USER, reg_key.data(), 0, KEY_ALL_ACCESS, &_registry_handle);

		if (status != ERROR_SUCCESS)
			return;

		std::generate_n(std::back_inserter(_registry_key), 16, []()
			{
				thread_local std::mt19937_64 mersenne_generator(std::random_device{ }());
				const std::uniform_int_distribution<> distribution(97, 122);
				return static_cast<std::uint8_t>(distribution(mersenne_generator));
			});

		const std::unique_ptr<std::remove_pointer_t<HANDLE>, decltype(&CloseHandle)> snap_shot(CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0), &CloseHandle);

		if (!snap_shot.get())
			return;

		PROCESSENTRY32 entry
		{
			sizeof(PROCESSENTRY32)
		};

		for (Process32First(snap_shot.get(), &entry); Process32Next(snap_shot.get(), &entry); )
			if (!std::strcmp(process.data(), entry.szExeFile))
				_process_id = static_cast<std::uintptr_t>(entry.th32ProcessID);

		_local_process_id = static_cast<std::uintptr_t>(GetCurrentProcessId());
	}

	void run(operation& request) const
	{
		operation_command command_request
		{
			_local_process_id,
			_process_id,
			secret_key,
			request,
			reinterpret_cast<std::uintptr_t>(&request)
		};

		RegSetValueExA(_registry_handle, _registry_key.c_str(), 0, REG_BINARY, reinterpret_cast<std::uint8_t*>(&command_request), sizeof(command_request));
	}

	template <typename T = std::uintptr_t> T read(const std::uintptr_t address)
	{
		T buffer{ };

		operation request
		{
			address,
			sizeof(T),
			reinterpret_cast<std::uintptr_t>(&buffer),
			0,
			0,
			operation_read
		};

		this->run(request);

		return buffer;
	}

	void read(const std::uintptr_t address, void* buffer, std::size_t size)
	{
		operation request
		{
			address,
			size,
			reinterpret_cast<std::uintptr_t>(buffer),
			0,
			0,
			operation_read
		};

		this->run(request);
	}

	template <typename T> void write(const std::uintptr_t address, T data)
	{
		operation request
		{
			address,
			sizeof(T),
			reinterpret_cast<std::uintptr_t>(&data),
			0,
			0,
			operation_write
		};

		this->run(request);
	}

	void write(std::uintptr_t address, void* buffer, std::size_t size) const
	{
		operation request
		{
			address,
			size,
			reinterpret_cast<std::uintptr_t>(buffer),
			0,
			0,
			operation_write
		};

		this->run(request);
	}

	void protect(std::uintptr_t address, std::size_t size, std::uint32_t new_protection, std::uint32_t* old_protection) const
	{
		operation request
		{
			address,
			size,
			0,
			new_protection,
			0,
			operation_protect
		};

		this->run(request);

		if (old_protection)
			*old_protection = request.old_protection;
	}

	std::uintptr_t base_address() const
	{
		operation request
		{
			0,
			0,
			0,
			0,
			0,
			operation_base
		};

		this->run(request);

		return request.buffer;
	}

	std::uintptr_t unity_module() const
	{
		operation request
		{
			0,
			0,
			0,
			0,
			0,
			operation_module
		};

		this->run(request);

		return request.buffer;
	}

	std::uintptr_t assembly_module() const
	{
		operation request
		{
			0,
			0,
			0,
			0,
			0,
			operation_module2
		};

		this->run(request);

		return request.buffer;
	}

	std::uintptr_t find_signature(const std::uintptr_t base_address, const char* sig, const char* mask)
	{
		const auto buffer = std::make_unique<std::array<std::uint8_t, 0x100000>>();
		auto data = buffer.get()->data();
		std::uintptr_t result = 0;

		for (std::uintptr_t i = 0u; i < (2u << 25u); ++i)
		{
			read(base_address + i * 0x100000, data, 0x100000);

			if (!data)
				return 0;

			for (std::uintptr_t j = 0; j < 0x100000u; ++j)
			{
				if ([](std::uint8_t const* data, std::uint8_t const* sig, char const* mask)
					{
						for (; *mask; ++mask, ++data, ++sig)
						{
							if (*mask == 'x' && *data != *sig) return false;
						}
						return (*mask) == 0;
					}(data + j, (std::uint8_t*)sig, mask))
				{
					result = base_address + i * 0x100000 + j;

					std::uint32_t rel = 0;

					read(result + 3, &rel, sizeof(std::uint32_t));

					if (!rel)
						return 0;

					return result - base_address + rel + 7;
				}
			}
		}

		return 0;
	}

	std::uintptr_t find_signature_rel(const char* sig, const char* mask)
	{
		auto buffer = std::make_unique<std::array<std::uint8_t, 0x100000>>();
		auto data = buffer.get()->data();

		for (std::uintptr_t i = 0u; i < (2u << 25u); ++i)
		{
			read(this->base_address() + i * 0x100000, data, 0x100000);

			if (!data)
				return 0;

			for (std::uintptr_t j = 0; j < 0x100000u; ++j)
			{
				if ([](std::uint8_t const* data, std::uint8_t const* sig, char const* mask)
					{
						for (; *mask; ++mask, ++data, ++sig)
						{
							if (*mask == 'x' && *data != *sig) return false;
						}
						return (*mask) == 0;
					}(data + j, (std::uint8_t*)sig, mask))
				{
					return i * 0x100000 + j;
				}
			}
		}

		return 0;
	}

	std::uintptr_t get_thread_info(HWND window) const
	{
		operation request
		{
			reinterpret_cast<std::uintptr_t>(window),
			0,
			0,
			0,
			0,
			operation_window_get
		};

		this->run(request);

		return request.buffer;
	}

	void set_thread_info(HWND window, std::uintptr_t thread_info) const
	{
		operation request
		{
			reinterpret_cast<std::uintptr_t>(window),
			0,
			thread_info,
			0,
			0,
			operation_window_set
		};

		this->run(request);
	}

	std::string read_ascii(const std::uintptr_t address, std::size_t size)
	{
		std::unique_ptr<char[]> buffer(new char[size]);
		read(address, buffer.get(), size);
		return std::string(buffer.get());
	}


	std::wstring read_unicode(const std::uintptr_t address, std::size_t size)
	{
		const auto buffer = std::make_unique<wchar_t[]>(size);
		read(address, buffer.get(), size * 2);
		return std::wstring(buffer.get());
	}

	void readString(DWORD64 Address, wchar_t* string)
	{
		if (read<uint16_t>(Address + 0x10) > 0)
		{
			for (int i = 0; i < read<uint16_t>(Address + 0x10) * 2; i++)
			{
				string[i] = read<wchar_t>(Address + 0x14 + (i * 2));
			}
		}
	}

	~memory_mgr()
	{
		_registry_key.clear(); _registry_key.shrink_to_fit(); _process_id = 0; CloseHandle(_registry_handle);
	}
};