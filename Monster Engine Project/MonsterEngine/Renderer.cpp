
#include "GLib.h"
#include "Renderer.h"

namespace Engine
{

	/*Renderer::Renderer()
	{

	}

	Renderer::~Renderer()
	{
		for (int i = 0; i < _renderItemList.size(); i++)
		{
			delete _renderItemList[i];
		}
	}

	bool Renderer::AddRenderedObject(Actor * i_Actor, const char * i_TexturePath)
	{
		RenderData * pNewData = new RenderData(i_Actor, i_TexturePath);
		_renderItemList.push_back(pNewData);

		return true;
	}

	void Renderer::Draw()
	{

		GLib::BeginRendering();
		GLib::Sprites::BeginRendering();

		for (int i = 0; i < _renderItemList.size(); i++) 
		{
			_renderItemList[i]->Draw();
		}

		GLib::Sprites::EndRendering();
		GLib::EndRendering();

	}*/

}//namespace Engine