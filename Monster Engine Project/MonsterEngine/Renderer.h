
#ifndef RENDERER_H
#define RENDERER_H

#include <vector>

#include "RenderData.h"

namespace Engine
{
	namespace Renderer
	{
		std::vector<RenderData *> * renderItemList;

		void Initialize()
		{
			renderItemList =  new std::vector<RenderData*>();
		}

		void ShutDown()
		{
			for (int i = 0; i < (*renderItemList).size(); i++)
			{
				delete (*renderItemList)[i];
			}

			delete renderItemList;
		}

		bool AddRenderedObject(SmartPointer<Actor> & i_Actor, const char * i_TexturePath)
		{
			RenderData * pNewData = new RenderData(i_Actor, i_TexturePath);
			(*renderItemList).push_back(pNewData);

			return true;
		}

		void Draw()
		{

			GLib::BeginRendering();
			GLib::Sprites::BeginRendering();

			for (int i = 0; i < (*renderItemList).size(); i++)
			{
				(*renderItemList)[i]->Draw();
			}

			GLib::Sprites::EndRendering();
			GLib::EndRendering();

		}


	}//namespace Renderer
	
}// namespace Engine

#endif //RENDERER_H
