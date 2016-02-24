#include <stdarg.h>	
#include <stdio.h>		
#include <stdarg.h>
#include <Windows.h>
#include "ConsolePrint.h"
#include "DebugLog.h"

namespace Engine{
	void DebugLog(const char * file, unsigned int line, const char * i_fmt, ...)
	{

		ConsolePrint("File: %s\n", file);
		ConsolePrint("Line: %d\n", line);

		const size_t lenTemp = 256;
		char strTemp[lenTemp] = "DEBUG: ";

		strcat_s(strTemp, i_fmt);

		const size_t lenOutput = lenTemp + 32;

		char strOutput[lenOutput];

		va_list args;
		va_start(args, i_fmt);
		vsprintf_s(strOutput, lenOutput, strTemp, args);
		va_end(args);

		OutputDebugStringA(strOutput);
	}
}