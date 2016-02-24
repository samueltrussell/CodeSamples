#include "MainGame.h"

#include <Windows.h>
#include <stdlib.h>

#include "Actor.h"
#include "BasicControl.h"
#include "DebugLog.h"
#include "GLib.h"
#include "LuaActorParser.h"
#include "Physics.h"
#include "PlayerController.h"
#include "Renderer.h"
#include "SmartPointer.h" //TODO
#include "StringPool.h"
#include "Timing.h"

Engine::StringPool * g_pStringPool;

MainGame::MainGame() 
{

}

MainGame::~MainGame() 
{

}

void MainGame::Initialize(HINSTANCE i_hInstance, int i_nCmdShow)
{

	g_pStringPool = Engine::StringPool::Create(1000);

	_bGLibSuccess = GLib::Initialize(i_hInstance, i_nCmdShow, "Semester 2 Game", NULL, 1080, 720);
	GLib::SetKeyStateChangeCallback(Engine::Control::KeyEventCallback);
	
	//Setup Physics
	Engine::Physics::Initialize(Vector3(0.0f, -9.8f, 0.0f));
	
	//Setup Renderer
	Engine::Renderer::Initialize();

	//Setup Timer
	Engine::Timing::Initialize();
}

void MainGame::Run()
{
	if (_bGLibSuccess) {

		bool bQuit = false;
		
		//Instantiate Game Objects
		Engine::SmartPointer<Engine::Actor> player = Engine::LuaParseActor("Data\\Actors\\Player.lua");
		Engine::SmartPointer<Engine::Actor> badGuy = Engine::LuaParseActor("Data\\Actors\\Villain.lua");

		//Setup Controls
		Engine::Control::RegisterPawn(player, 400.0f);
		
		double lastFrameTime;

		int loopCount = 0;

		do
		{
			GLib::Service(bQuit);

			if (!bQuit) {

				loopCount++;

				lastFrameTime = Engine::Timing::CalcLastFrameTime();

				//Uncomment for frame time print
				//DEBUG_LOG("Last Frame Time in ms: %f", lastFrameTime);

				Engine::Physics::Update(lastFrameTime/1000.0f);

				Engine::Renderer::Draw();

#ifdef MAX_FRAME_RATE
				double currentFrameTime = Engine::Timing::CalcCurrentFrameTime();
				if (Engine::Timing::minFrameTime > currentFrameTime) {
					DWORD sleeptime = (DWORD)(Engine::Timing::minFrameTime - currentFrameTime);
					Sleep(sleeptime);
				}
#endif

			}

		} while (bQuit != true); 
	}


	delete g_pStringPool;
	Engine::Physics::ShutDown();
	Engine::Renderer::ShutDown();
	Engine::Control::ShutDown();
	GLib::Shutdown();
}