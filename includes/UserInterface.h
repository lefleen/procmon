#pragma once

#include <iostream>

#include "types.h"

namespace UserInterface
{
	namespace ShowHelp
	{
		void full_help();
	}

	namespace ShowSettings
	{
		void name(bool status);

		void time(bool status);

		void memory(bool status);

		void total_cpu(bool status);

		void interval_cpu(bool status);
	}

	namespace ShowAssert 
	{
		void time_off();
	}
}