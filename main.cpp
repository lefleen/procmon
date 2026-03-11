#include "ManageProgramm.h"
#include "iostream"

int main() 
{
	while(ManageProgramm::start_programm() == Result::failure);

	return 0;
}
