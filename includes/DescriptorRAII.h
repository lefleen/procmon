#pragma once

#ifdef __linux__
#include <unistd.h>

#endif 

#include "types.h"

class DescriptorRAII
{
	descriptor_process_t _descriptor_process = null_t;

	void CloseDescriptor(descriptor_process_t);
public:
	explicit DescriptorRAII(descriptor_process_t) noexcept;

	explicit DescriptorRAII(DescriptorRAII&&) noexcept;

	DescriptorRAII(DescriptorRAII&) noexcept = delete;

	DescriptorRAII() noexcept = default;

	~DescriptorRAII();

	descriptor_process_t get() const noexcept;

	DescriptorRAII& operator=(descriptor_process_t descriptor_process);
};
