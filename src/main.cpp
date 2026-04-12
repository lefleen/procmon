#include "ManageProgramm.h"
#include "CommandProcessor.h"

int main(int argc, const char* argv[]) 
{
    Result res;
    ProcmonSettings procmon_settings = { };
    vec_t<ProcmonSettingsTable> procmon_settings_table = { };
    set_table_settings(procmon_settings_table, procmon_settings);

    if ((res = CommandProcessor::manage(procmon_settings_table, argc, argv)) == Result::failure) return -1;
    else if (res == Result::invalid_arguments)
        return -1;
    else if (res != Result::no_arguments)
        return 0;

    int count = 0;

	for (; count <= 5; ++count)
		if (ManageProgramm::start_programm(procmon_settings) == Result::successful) break;
	if (count == 6) return -1;

	return 0;
}
