//Inline function definition for the Vector3 class

namespace Engine
{
	inline SmartPointer<Actor> PhysicsInfo::getActorPtr()
	{
		return _pActor;
	}

	inline float const PhysicsInfo::Mass() const
	{
		return _mass;
	}

	inline float const PhysicsInfo::Friction() const
	{
		return _friction;
	}

	inline float const PhysicsInfo::Drag() const
	{
		return _drag;
	}

	inline float const PhysicsInfo::GravityScalar() const
	{
		return _gravityScalar;
	}



}
