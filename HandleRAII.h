#pragma once
#include "libraries.h"

class HandleRAII
{
	HANDLE _handle_process = nullptr;
public:
	explicit HandleRAII(HANDLE) noexcept;

	explicit HandleRAII(HandleRAII&&) noexcept;

	HandleRAII(HandleRAII&) noexcept = delete;

	HandleRAII() noexcept = default;

	~HandleRAII();

	HANDLE get() const noexcept;

	HandleRAII& operator=(HANDLE hProcess);
};
