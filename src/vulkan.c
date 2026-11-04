#include "vulkan.h"

void logMessage(LogLevel level, const char* format, ...)
{
    FILE* stream;
    if (level == LOG_NONE) return;

    stream = (level == LOG_ERROR) ? stderr : stdout;
    if (level < LOG_INFO || level > LOG_ERROR)
	return;

    const char* levelName[] = {
	[LOG_INFO] = "INFO",
	[LOG_WARNING] = "WARNING",
	[LOG_ERROR] = "ERROR"
    };

    fprintf(stream, "[%s] ", levelName[level]);
    va_list args;
    va_start(args, format);
    vfprintf(stream, format, args);
    va_end(args);
    fflush(stream);
} 
