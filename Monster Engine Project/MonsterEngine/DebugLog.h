#ifndef DEBUG_LOG_H
#define DEBUG_LOG_H

namespace Engine
{

	void DebugLog(const char * , unsigned int , const char *, ...);

}


#if defined(_DEBUG)  &&  !defined(DISABLE_DEBUG_PRINT)

#define DEBUG_LOG(fmt,...) Engine::DebugLog(__FILE__, __LINE__, fmt, __VA_ARGS__)

#else
#define DEBUG_LOG(fmt,...) void(0)
#endif

#endif //DEBUG_LOG_H