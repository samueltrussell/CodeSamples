//Inline function definition for the Vector3 class

inline float const Vector3::GetX() const
{
	return x;
}

inline float const Vector3::GetY() const
{
	return y;
}

inline float const Vector3::GetZ() const
{
	return z;
}

inline void Vector3::SetParameters(float i_x, float i_y, float i_z)
{
	x = i_x;
	y = i_y;
	z = i_z;
}

inline void Vector3::SetParameters(float i_x, float i_y)
{
	x = i_x;
	y = i_y;
	z = 0;
}

inline void Vector3::SetX(float i_x)
{
	x = i_x;
}

inline void Vector3::SetY(float i_y)
{
	y = i_y;
}

inline void Vector3::SetZ(float i_z)
{
	z = i_z;
}

Vector3 Vector3::operator^(const float i_exponent) const
{
	return ElementWisePow(i_exponent);
}