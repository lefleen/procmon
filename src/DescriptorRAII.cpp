#include "DescriptorRAII.h"

DescriptorRAII::DescriptorRAII(descriptor_process_t descriptor_process) noexcept : _descriptor_process(descriptor_process) { };

DescriptorRAII::DescriptorRAII(DescriptorRAII&& other) noexcept : _descriptor_process(other._descriptor_process)
{
	other._descriptor_process = null_t;
}

DescriptorRAII::~DescriptorRAII()
{
	CloseDescriptor(_descriptor_process);
}


descriptor_process_t DescriptorRAII::get() const noexcept
{
	return _descriptor_process;
}

void DescriptorRAII::CloseDescriptor(descriptor_process_t descriptor_process) 
{
	if (descriptor_process == null_t) return;
#ifdef _WIN32
	CloseHandle(descriptor_process);
#elif defined __linux__
	close(descriptor_process);
#endif
}

DescriptorRAII& DescriptorRAII::operator=(descriptor_process_t descriptor_process)
{
	if (_descriptor_process != descriptor_process)
	{
        if(_descriptor_process != null_t)
  		    CloseDescriptor(_descriptor_process);

		_descriptor_process = descriptor_process;
	}
	return *this;
}
