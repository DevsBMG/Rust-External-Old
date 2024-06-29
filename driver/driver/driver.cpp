#include "driver.hpp"

auto memory::setup(std::uint32_t process_id) -> void
{
	//driver setup
}

auto memory::read_buffer(PVOID Address, PVOID Buffer, SIZE_T Size) -> void
{
	//read buffer
	ReadProcessMemory(this->handle, Address, Buffer, Size, nullptr);
}

auto memory::write_buffer(PVOID Address, PVOID Buffer, SIZE_T Size) -> void
{
	//write buffer
	WriteProcessMemory(this->handle, Address, Buffer, Size, nullptr);
}