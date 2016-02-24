//This file contains the definitions for functions used to create the MessagedAssert Functionality

#include <stdio.h>
#include <Windows.h>

#include "ConsolePrint.h"
#include "MessagedAssert.h"

bool Engine::MessagedAsssertAction(const char * condition, const char * message, const char * file, unsigned int line)
{
	const size_t sizeMessage = 1024;
	char Message[sizeMessage];

	sprintf_s(Message, sizeMessage, "ASSERT: %s\n\nFile: %s \n\nLine: %d\n\nMessage: %s\n\nShould we try to continue ?",
		condition, file, line, message);

	int result = MessageBoxA(NULL, Message, "Assertion Failed!", MB_YESNO);

	switch (result)
	{
	case IDYES:
		Engine::ConsolePrint("File: %s Line: %d Condition: %s\n", file, line, condition);
		Engine::ConsolePrint("Message: %s\n", message);
		return true;
	default:
		return false;
	}
}