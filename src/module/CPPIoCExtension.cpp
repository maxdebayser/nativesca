

#include <stdlib.h>
#include <tuscany/sca/core/SCARuntime.h>
#include "IoCImplementationExtension.h"
#include "IoCInterfaceExtension.h"

using namespace tuscany::sca;

extern "C"
{
#if defined(WIN32) || defined(_WINDOWS)
	__declspec(dllexport)
#endif
		void tuscany_sca_ioc_initialize()
	{
		SCARuntime* runtime = SCARuntime::getCurrentRuntime();
		runtime->registerImplementationExtension(new IoCImplementationExtension());
		runtime->registerInterfaceExtension(new IoCInterfaceExtension());
	}
}
