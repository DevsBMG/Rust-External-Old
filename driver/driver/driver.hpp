#pragma once
#include <vector>
#include <Windows.h>
#include <functional>
#include <string>
#include "estr.hpp"

class memory
{
public:
	HANDLE handle;
public:
	memory(HANDLE HANDLE)
		: handle(HANDLE)
	{
		handle = HANDLE;
	}

	void setup(std::uint32_t process_id);

	void read_buffer(PVOID Address, PVOID Buffer, SIZE_T Size);
	void write_buffer(PVOID Address, PVOID Buffer, SIZE_T Size);

	template <typename T>
	auto write(std::uintptr_t Address, T buffer) -> void
	{
		this->write_buffer((PVOID)Address, &buffer, sizeof(T));
	}

	template <typename T>
	auto read(std::uintptr_t Address) -> T&
	{
		T buffer;
		this->read_buffer((PVOID)Address, &buffer, sizeof(T));
		return buffer;
	}

	auto read_string(std::uintptr_t addr) -> std::string
	{
		char buffer[128];
		this->read_buffer((PVOID)addr, (std::uint8_t*)&buffer, sizeof(buffer));
		return std::string(buffer);
	}

	auto read_wstring(std::uintptr_t addr) -> std::wstring
	{
		wchar_t buffer[128];
		this->read_buffer((PVOID)addr, (std::uint8_t*)&buffer, sizeof(buffer));
		return std::wstring(buffer, wcslen(buffer));
	}

	template <typename T>
	auto read_chain(std::uintptr_t Address, const std::vector<std::uintptr_t>& Chain) -> T&
	{
		SIZE_T Size = Chain.size();
		for (SIZE_T i = 0; i < Size - 1; ++i) Address = read<ULONG64>(Address + Chain[i]);
		return read<T>(Address + Chain[Size - 1]);
	}
public:
	std::uintptr_t game_assembly;
	std::uintptr_t unity_player;
};