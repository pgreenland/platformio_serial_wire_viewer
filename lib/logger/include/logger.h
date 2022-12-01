#ifndef LOGGER_H
#define LOGGER_H

/* Standard libraries */
#include <inttypes.h>

/* Define log levels */
typedef enum
{
	eLOGGER_Level_None,
	eLOGGER_Level_Error,
	eLOGGER_Level_Warn,
	eLOGGER_Level_Info,
	eLOGGER_Level_Debug,
	eLOGGER_Level_Verbose

} eLOGGER_Level_t;

/* Define global log level */
#ifndef LOGGER_LEVEL
#define LOGGER_LEVEL (eLOGGER_Level_Info)
#endif

/* Define log level */
#ifndef LOGGER_LOCAL_LEVEL
#define LOGGER_LOCAL_LEVEL (LOGGER_LEVEL)
#endif

/* Define common log function */
#define mLOGGER_COMMON(level, level_str, tag, format, ...) do { \
	if ((LOGGER_LOCAL_LEVEL) >= (level)) LOGGER_Print(level_str"%s: "format"\n", tag, ##__VA_ARGS__); \
} while(0)

/* Define log helpers */
#define LOGGER_V(tag, format, ...) mLOGGER_COMMON(eLOGGER_Level_Verbose, "V: ", tag, format, ##__VA_ARGS__)
#define LOGGER_D(tag, format, ...) mLOGGER_COMMON(eLOGGER_Level_Debug, "D: ", tag, format, ##__VA_ARGS__)
#define LOGGER_I(tag, format, ...) mLOGGER_COMMON(eLOGGER_Level_Info, "I: ", tag, format, ##__VA_ARGS__)
#define LOGGER_W(tag, format, ...) mLOGGER_COMMON(eLOGGER_Level_Warn, "W: ", tag, format, ##__VA_ARGS__)
#define LOGGER_E(tag, format, ...) mLOGGER_COMMON(eLOGGER_Level_Error, "E: ", tag, format, ##__VA_ARGS__)

/* Define weak common logger function, which will use printf */
void LOGGER_Print(const char* pszFormat, ...);

#endif
