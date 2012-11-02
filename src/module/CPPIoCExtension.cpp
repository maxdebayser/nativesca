#include <iostream>
using namespace std;

#include <stdlib.h>
#include "tuscany/sca/core/SCARuntime.h"

using namespace tuscany::sca;

extern "C"
{
#if defined(WIN32) || defined(_WINDOWS)
	__declspec(dllexport)
#endif
		void tuscany_sca_ioc_initialize()
	{
		SCARuntime* runtime = SCARuntime::getCurrentRuntime();
		std::cout << "Hello runtime " << ((void*)runtime)  << std::endl;

	}
}
