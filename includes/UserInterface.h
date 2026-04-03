#pragma once

#include <iostream>

#include "types.h"

namespace UserInterface
{
	namespace ShowHelp
	{
		void all();
	}

	namespace ShowSettings
	{
		void name();

		void time();

		void memory();

		void total_cpu();

		void interval_cpu();
	}

	namespace ShowAssert 
	{
		void time_off();
	}
}