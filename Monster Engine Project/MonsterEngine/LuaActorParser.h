
#ifndef LUA_ACTOR_PARSER_H
#define LUA_ACTOR_PARSER_H

#include <assert.h>
#include <stdint.h>
#include <string>

#include "Physics.h"
#include "Renderer.h"
#include "lua.hpp"
#include "Vector3.h"

namespace Engine {

	//Forward Declarations
	uint8_t * LoadFile(const char * i_pFileName, size_t & o_sizeFile);
	Vector3 LuaGetVector3(lua_State * i_pLuaState, const char * i_pVectorName);

	
	SmartPointer<Actor> LuaParseActor(const char * i_pFileName)
	{
		//create a lua_State
		lua_State * pLuaState = luaL_newstate();
		assert(pLuaState);

		luaL_openlibs(pLuaState);

		//Read in a file
		//size_t fileSize = 0;
		//uint8_t * pFileData = LoadFile(i_pFileName, fileSize);
		
		int result = 0;
		
		//TODO branchpoint-decide whether to use joes at this point
		result = luaL_loadfile(pLuaState, i_pFileName);
		assert(result == 0);
		lua_pcall(pLuaState, 0, 0, 0);
		assert(result == 0);

		if (result == 0) {

			int type = LUA_TNIL;

			//move the player to the stack
			result = lua_getglobal(pLuaState, "Actor");
			assert(result == LUA_TTABLE);

			//push "name" onto stack to request from table
			lua_pushstring(pLuaState, "name");
			result = lua_gettable(pLuaState, -2);
			assert(result == LUA_TSTRING);

			const char * pName = lua_tostring(pLuaState, -1);
			assert(pName);
			if (pName)
				pName = _strdup(pName);

			//pop the stack to clear the name stuff...
			lua_pop(pLuaState, 1);

			//push "class" onto the stack to request from table
			lua_pushstring(pLuaState, "class");
			result = lua_gettable(pLuaState, -2);
			assert(result == LUA_TSTRING);

			const char * pClass = lua_tostring(pLuaState, -1);
			assert(pClass);
			if (pClass)
				pClass = _strdup(pClass);

			//pop the stack to clear the class stuff...
			lua_pop(pLuaState, 1);

			//push "controller" onto the stack to request from table
			lua_pushstring(pLuaState, "controller");
			result = lua_gettable(pLuaState, -2);
			assert(result == LUA_TSTRING);

			const char * pController = lua_tostring(pLuaState, -1);
			assert(pController);
			if (pController)
				pController = _strdup(pController);

			//pop the stack to clear the controller stuff...
			lua_pop(pLuaState, 1);

			//pull the initialPosition vector
			Vector3 initialPosition = LuaGetVector3(pLuaState, "initialPosition");

			//pull the initialVelocity vector
			Vector3 initialVelocity = LuaGetVector3(pLuaState, "initialVelocity");

			//============= get the physics info on the stack ========================
			bool hasPhysics = false;

			lua_pushstring(pLuaState, "physicsInfo");
			result = lua_gettable(pLuaState, -2);
			
			float mass, friction, drag, gravity;

			if (result != 0) {
				assert(result == LUA_TTABLE);

				hasPhysics = true;

				//push "mass" onto the stack to request from table
				lua_pushstring(pLuaState, "mass");
				result = lua_gettable(pLuaState, -2);
				assert(result == LUA_TNUMBER);

				mass = (float)lua_tonumber(pLuaState, -1);

				//pop the stack to clear the mass stuff...
				lua_pop(pLuaState, 1);

				//push "friction" onto the stack to request from table
				lua_pushstring(pLuaState, "friction");
				result = lua_gettable(pLuaState, -2);
				assert(result == LUA_TNUMBER);

				friction = (float)lua_tonumber(pLuaState, -1);

				//pop the stack to clear the friction stuff...
				lua_pop(pLuaState, 1);

				//push "drag" onto the stack to request from table
				lua_pushstring(pLuaState, "drag");
				result = lua_gettable(pLuaState, -2);
				assert(result == LUA_TNUMBER);

				drag = (float)lua_tonumber(pLuaState, -1);

				//pop the stack to clear the drag stuff...
				lua_pop(pLuaState, 1);

				//push "gravityScalar" onto the stack to request from table
				lua_pushstring(pLuaState, "gravityScalar");
				result = lua_gettable(pLuaState, -2);
				assert(result == LUA_TNUMBER);

				gravity = (float)lua_tonumber(pLuaState, -1);

				//pop the stack twice to clear the gravity 
				lua_pop(pLuaState, 1);
			}
			
			//Pop physics stuff (or error code...) off the stack to get back down to the Actor object
			lua_pop(pLuaState, 1);
			

			//=========== get the renderData object on the stack ================
			bool isRenderable = false;

			lua_pushstring(pLuaState, "renderData");
			result = lua_gettable(pLuaState, -2);

			const char * pSpritePath = "";

			if (result != 0) {
				assert(result == LUA_TTABLE);

				isRenderable = true;

				//push "spriteFile" onto the stack to request from table
				lua_pushstring(pLuaState, "spriteFile");
				result = lua_gettable(pLuaState, -2);
				assert(result == LUA_TSTRING);

				pSpritePath = lua_tostring(pLuaState, -1);
				assert(pSpritePath);
				if (pSpritePath)
					pSpritePath = _strdup(pSpritePath);

				//pop the stack to clear the spriteFile stuff...
				lua_pop(pLuaState, 1);
			}

			//pop the stack to clear the renderData (or error code...) to get back down to the Actor object
			lua_pop(pLuaState, 1);

			//pop the stack to clear the Actor Object
			lua_pop(pLuaState, 1);

			int stackCounter = lua_gettop(pLuaState);
			assert(stackCounter == 0);

			//Wipe out the LuaState to clean up the memory mess
			lua_close(pLuaState);

			//Instantiate an Actor and register it with physics and renderer as appropriate
			//TODO: this code will likely move elsewhere as multi-threading is integrated

			SmartPointer<Actor> actor(new Actor(initialPosition, initialVelocity, pName, pClass));

			//free anything that was _strdup'ed to create the Actor
			free(const_cast<char*>(pName));
			free(const_cast<char*>(pClass));
			free(const_cast<char*>(pController));

			if (hasPhysics) {
				Engine::Physics::AddPhysicsObject(actor, mass, friction, drag, gravity);
				//free anything that was _strdup'ed to add physics
			}

			if (isRenderable) {
				Engine::Renderer::AddRenderedObject(actor, pSpritePath);
				//free anything that was _strdup'ed to make Renderable
				free(const_cast<char*>(pSpritePath));
			}


			return actor;

		}

		return NULL;

	}//ParseActor

	Vector3 LuaGetVector3(lua_State * i_pLuaState, const char * i_pVectorName) 
	{
		Vector3 newVector;

		float tempVector[3] = { 0.0f };

		int result = 0;

		lua_pushstring(i_pLuaState, i_pVectorName);
		result = lua_gettable(i_pLuaState, -2);
		assert(result == LUA_TTABLE);

		lua_pushnil(i_pLuaState);

		for (int i = 0; i < 3; i++) {
			
			lua_next(i_pLuaState, -2);
			assert(lua_type(i_pLuaState, -1) == LUA_TNUMBER);

			tempVector[i] = (float)lua_tonumber(i_pLuaState, -1);

			lua_pop(i_pLuaState, 1);
			
		}

		lua_pop(i_pLuaState, 2);

		newVector.SetParameters(tempVector[0], tempVector[1], tempVector[2]);

		return newVector;
	}//LuaGetVector

	uint8_t * LoadFile(const char * i_pFileName, size_t & o_sizeFile) 
	{

		assert(i_pFileName != NULL);

		FILE * pFile = NULL;

		errno_t fOpenError = fopen_s(&pFile, i_pFileName, "rb");
		if (fOpenError != 0)
			return NULL;

		assert(pFile != NULL);

		int fileIOError = fseek(pFile, 0, SEEK_END);
		assert(fileIOError == 0);

		size_t fileSize = ftell(pFile);
		assert(fileSize >= 0);

		fileIOError = fseek(pFile, 0, SEEK_SET);
		assert(fileIOError == 0);

		uint8_t * pBuffer = new uint8_t[fileSize];
		assert(pBuffer);

		size_t fileRead = fread(pBuffer, 1, fileSize, pFile);
		assert(fileRead == fileSize);

		fclose(pFile);

		o_sizeFile = fileSize;

		return pBuffer;

	}//LoadFile
	
}//namespace Engine

#endif //LUA_ACTOR_PARSER_H
