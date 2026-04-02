#include "ManageProgramm.h"
#include "CommandProcessor.h"

int main(int argc, const char* argv[]) 
{
    Result res;
    ProcmonSettings procmon_settings { };
    if((res = CommandProcessor::parse_string(procmon_settings, argc, argv)) == Result::failure) return -1; 
    else if(res == Result::invalid_arguments) 
    { 
        /*UserInterface::show_err(res)*/
        return -1;
    }

    int count = 0;

	for (; count <= 5; ++count)
		if (ManageProgramm::start_programm() == Result::successful) break;
	if (count == 6) return -1;

	return 0;
}
