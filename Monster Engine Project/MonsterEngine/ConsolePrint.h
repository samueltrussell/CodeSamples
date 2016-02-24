#ifndef CONSOLE_PRINT_H
#define CONSOLE_PRINT_H

namespace Engine
{

	void ConsolePrint(const char * i_fmt, ...);

} // Namespace Engine

#if defined(_DEBUG)  &&  !defined(DISABLE_DEBUG_PRINT)

#define DEBUG_PRINT(fmt,...) Engine::ConsolePrint((fmt), __VA_ARGS__)

#else
#define DEBUG_PRINT(fmt,...) void(0)
#endif


#endif // CONSOLE_PRINT_H