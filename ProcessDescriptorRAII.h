#pragma once

#ifdef __linux__
#include <fcntl.h>

#endif 

#include "types.h"

class ProcessDescriptorRAII
{
	descriptor_process_t _descriptor_process = null_t;

	void CloseDescriptor(descriptor_process_t);
public:
	explicit ProcessDescriptorRAII(descriptor_process_t) noexcept;

	explicit ProcessDescriptorRAII(ProcessDescriptorRAII&&) noexcept;

	ProcessDescriptorRAII(ProcessDescriptorRAII&) noexcept = delete;

	ProcessDescriptorRAII() noexcept = default;

	~ProcessDescriptorRAII();

	descriptor_process_t get() const noexcept;

	ProcessDescriptorRAII& operator=(descriptor_process_t descriptor_process);
};
