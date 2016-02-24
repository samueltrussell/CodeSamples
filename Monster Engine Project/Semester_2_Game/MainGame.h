#ifndef MAIN_GAME_H
#define MAIN_GAME_H

#include <Windows.h>

class MainGame
{
public:
	MainGame();
	~MainGame();
	void Initialize(HINSTANCE i_hInstance, int i_nCmdShow);
	void Run();

private:
	bool _bGLibSuccess;

};

#endif // MAIN_GAME_H

