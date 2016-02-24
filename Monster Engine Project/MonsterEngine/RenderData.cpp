#include "RenderData.h"

#include "GLibHelperFunctions.h"

namespace Engine {

	RenderData::RenderData(SmartPointer<Actor> & i_pActor, const char * i_SpritePath) :
		_pActor(i_pActor)
	{
		SetSprite(i_SpritePath);
	}

	RenderData::~RenderData()
	{
		GLib::Sprites::Release(_pSprite);
	}

	bool RenderData::SetSprite(const char * i_SpritePath)
	{
		if (_pSprite != nullptr) {
			GLib::Sprites::Release(_pSprite);
		}

		_pSprite = CreateSprite(i_SpritePath);

		if (_pSprite != nullptr)
			return true;

		return false;
	}

	void RenderData::Draw()
	{
		GLib::Point2D drawLocation = { _pActor->position.GetX(), _pActor->position.GetY() };
		GLib::Sprites::RenderSprite(*_pSprite, drawLocation);
	}
}