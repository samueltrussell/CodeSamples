
#ifndef RENDER_DATA_H
#define RENDER_DATA_H

#include "Actor.h"
#include "GLib.h"
#include "SmartPointer.h"

namespace Engine {
	
	class RenderData
	{
	public:
		~RenderData();
		RenderData(SmartPointer<Actor> & i_pActor, const char * i_SpritePath);
		bool SetSprite(const char * i_SpritePath);
		void Draw();
	private:
		GLib::Sprites::Sprite * _pSprite;
		SmartPointer<Actor> _pActor;

	};

}//namespace Engine

#endif //RENDER_DATA_H
