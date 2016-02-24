#include <conio.h>
#include <iostream>

#include "StringPool.h"

bool StringPoolUnitTests() {


	Engine::StringPool * pStringPool = Engine::StringPool::Create(50);

	const char * name1 = pStringPool->add("Joe");
	std::cout << name1 << std::endl;

	const char * name2 = pStringPool->add("Fred");
	std::cout << name2 << std::endl;

	const char * name3 = pStringPool->add("Joe");
	std::cout << name3 << std::endl;

	pStringPool->find("Fred");

	_getch();

	return true;
}