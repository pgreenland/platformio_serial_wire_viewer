/* Standard libraries */
#include <stdio.h>
#include <stdarg.h>

/* Public header file */
#include "logger.h"

__attribute__((weak)) void LOGGER_Print(const char* pszFormat, ...)
{
	va_list xArgs;
	va_start(xArgs, pszFormat);
	vprintf(pszFormat, xArgs);
	va_end(xArgs);
}
