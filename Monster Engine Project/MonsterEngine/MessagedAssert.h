//This Header declares Macros and functions for creating a Messaged Assert Command

#ifndef MESSAGED_ASSERT_H
#define MSSAGED_ASSERT_H

namespace Engine
{
	bool MessagedAsssertAction(const char *, const char *, const char *, unsigned int);
}

#ifdef _DEBUG

#define MessagedAssert(condition,message) if( (!condition) && (!Engine::MessagedAsssertAction(#condition, message, __FILE__, __LINE__)) ) {__debugbreak();}

#else
#define MessagedAssert(condintion, message) ((void)0)
#endif

#endif //MESSAGED_ASSER_H
