/* System includes */
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>

/* Hardware support */
#include <libopencm3/cm3/itm.h>
#include <libopencm3/stm32/dbgmcu.h>

/* Logging */
#include <logger.h>

/* Use ITM port 0 for printf messages */
#define ITM_STIM_PORT_PRINTF (0)

/* Public function definitions */
int _write(int fd, char *buf, int count);

/* Implementation of _write for printf */
int _write (int fd, char *buf, int count)
{
	/* Only support stdout / stderr */
	if ((STDOUT_FILENO == fd) || (STDERR_FILENO == fd))
	{
		/* Check ITM and stimulus port enabled */
		if (	(ITM_TCR & ITM_TCR_ITMENA)
			 && (0 != ITM_TER[ITM_STIM_PORT_PRINTF])
		   )
		{
			/* Write to ITM port */
			for (int iIndex = 0; iIndex < count; iIndex++)
			{
				/* Enqueue byte in ITM FIFO */
				while (!(ITM_STIM8(ITM_STIM_PORT_PRINTF) & ITM_STIM_FIFOREADY));
				ITM_STIM8(ITM_STIM_PORT_PRINTF) = buf[iIndex];
			}
		}

		/* All data written */
		return count;
	}

	/* IO error, stdin / unknown stream */
	errno = EIO;
	return -1;
}

/* Override logger print function, checking port is enabled before formatting */
void LOGGER_Print(const char* pszFormat, ...)
{
	va_list xArgs;

	/* Check ITM and stimulus port are enabled before calling printf for formatting operation */
	if (	(ITM_TCR & ITM_TCR_ITMENA)
			&& (0 != ITM_TER[ITM_STIM_PORT_PRINTF])
	   )
	{
		va_start(xArgs, pszFormat);
		vprintf(pszFormat, xArgs);
		va_end(xArgs);
	}
}
