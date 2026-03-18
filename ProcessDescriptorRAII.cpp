#include "ProcessDescriptorRAII.h"

ProcessDescriptorRAII::ProcessDescriptorRAII(descriptor_process_t descriptor_process) noexcept : _descriptor_process(descriptor_process) { };

ProcessDescriptorRAII::ProcessDescriptorRAII(ProcessDescriptorRAII&& other) noexcept : _descriptor_process(other._descriptor_process)
{
	other._descriptor_process = null_t;
}

ProcessDescriptorRAII::~ProcessDescriptorRAII()
{
	CloseDescriptor(_descriptor_process);
}


descriptor_process_t ProcessDescriptorRAII::get() const noexcept
{
    lseek(_descriptor_process, 0, SEEK_SET);
	return _descriptor_process;
}

void ProcessDescriptorRAII::CloseDescriptor(descriptor_process_t descriptor_process) 
{
	if (descriptor_process == null_t) return;
#ifdef _WIN32
	CloseHandle(descriptor_process);
#elif defined __linux__
	close(descriptor_process);
#endif
}

ProcessDescriptorRAII& ProcessDescriptorRAII::operator=(descriptor_process_t descriptor_process)
{
	if (_descriptor_process != descriptor_process)
	{
        if(_descriptor_process != null_t)
  		    CloseDescriptor(_descriptor_process);

		_descriptor_process = descriptor_process;
	}
	return *this;
}
