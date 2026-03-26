#include "ManageProgramm.h"
#include "iostream"

int main() 
{
	int count = 0;
	for (; count <= 5; ++count)
		if (ManageProgramm::start_programm() == Result::successful) break;
	if (count == 6) return -1;

	return 0;
}
