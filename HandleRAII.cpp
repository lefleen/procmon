#include "libraries.h"

HandleRAII::HandleRAII(HANDLE handle_process) noexcept : _handle_process(handle_process) { };

HandleRAII::HandleRAII(HandleRAII&& other) noexcept : _handle_process(other._handle_process)
{
	other._handle_process = nullptr;
}

HandleRAII::~HandleRAII()
{
	if (_handle_process)
		CloseHandle(_handle_process);
}

HANDLE HandleRAII::get() const noexcept
{
	return _handle_process;
}

HandleRAII& HandleRAII::operator=(HANDLE hProcess)
{
	if (_handle_process != hProcess)
	{
		if (_handle_process)
			CloseHandle(_handle_process);
		_handle_process = hProcess;
	}
	return *this;
}
