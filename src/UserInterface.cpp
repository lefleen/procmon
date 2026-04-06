#include "UserInterface.h"

void UserInterface::ShowHelp::full_help() 
{
	std::cout << "test help" << std::endl;
}

void UserInterface::ShowSettings::name(bool status) 
{
	std::cout << "name:";
	if (status) std::cout << "on";
	else std::cout << "off";
	std::cout << std::endl;
}

void UserInterface::ShowSettings::time(bool status)
{
	std::cout << "time:";
	if (status) std::cout << "on";
	else std::cout << "off";
	std::cout << std::endl;
}

void UserInterface::ShowSettings::memory(bool status)
{
	std::cout << "memory:";
	if (status) std::cout << "on";
	else std::cout << "off";
	std::cout << std::endl;
}

void UserInterface::ShowSettings::total_cpu(bool status)
{
	std::cout << "totalCPU:";
	if (status) std::cout << "on";
	else std::cout << "off";
	std::cout << std::endl;
}

void UserInterface::ShowSettings::interval_cpu(bool status)
{
	std::cout << "intervalCPU:";
	if (status) std::cout << "on";
	else std::cout << "off";
	std::cout << std::endl;
}

void UserInterface::ShowAssert::time_off()
{
	std::cout << "If you off calculate time, calculate interval cpu too off" << std::endl;
}