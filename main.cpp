#include "libraries.h"

int main() 
{
	auto start = std::chrono::steady_clock::now();

	ManageProgramm::start_programm();

	auto diff = std::chrono::steady_clock::now() - start;

	std::cout << std::chrono::duration<double, std::milli>(diff).count() << " ms" << std::endl;


	return 0;
}