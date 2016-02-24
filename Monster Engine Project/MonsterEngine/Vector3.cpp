//Member Definitions for Vector3 
#include "Vector3.h"

#include <cmath>
#include <iostream>

#include "FloatUtilities.h"

Vector3::Vector3(float i_x, float i_y, float i_z) :
x(i_x),
y(i_y),
z(i_z)
{
}

void Vector3::Print() const
{
	printf("x = %1.15f y = %1.15f z = %1.15f\n", x, y, z);
}

bool Vector3::operator==(const Vector3 & otherVector) const
{
	using namespace Engine;

	if (AboutEqualsRelative( x, otherVector.GetX(), .0002f) && 
		AboutEqualsRelative( y, otherVector.GetY(), .0002f) && 
		AboutEqualsRelative( z, otherVector.GetZ(), .0002f))
	{
		return true;
	}
	else
		return false;
}

void Vector3::operator=(const Vector3 & otherVector)
{
	x = otherVector.GetX();
	y = otherVector.GetY();
	z = otherVector.GetZ();
}

Vector3 Vector3::operator+(const Vector3 & otherVector) const
{
	Vector3 sum;
	sum.SetParameters(GetX() + otherVector.GetX(), GetY() + otherVector.GetY(), GetZ() + otherVector.GetZ());
	return sum;
}

void Vector3::operator+=(const Vector3 & otherVector)
{
	SetParameters(GetX() + otherVector.GetX(), GetY() + otherVector.GetY(), GetZ() + otherVector.GetZ());
}

Vector3 Vector3::operator-(const Vector3 & otherVector) const
{
	Vector3 difference;
	difference.SetParameters(GetX() - otherVector.GetX(), GetY() - otherVector.GetY(), GetZ() - otherVector.GetZ());
	return difference;
}

void Vector3::operator-=(const Vector3 & otherVector)
{
	SetParameters(GetX() - otherVector.GetX(), GetY() - otherVector.GetY(), GetZ() - otherVector.GetZ());
}

Vector3 Vector3::operator*(const float & scalar) const
{
	Vector3 product;
	product.SetParameters(GetX() * scalar, GetY() * scalar, GetZ() * scalar);
	return product;
}

float Vector3::Dot(const Vector3 & otherVector) const
{
	float dotProduct;
	dotProduct = (GetX() * otherVector.GetX() + GetY() * otherVector.GetY() + GetZ() * otherVector.GetZ());
	return dotProduct;
}

Vector3 Vector3::Cross(const Vector3 & otherVector) const
{
	Vector3 crossProduct;
	crossProduct.SetParameters(GetY()*otherVector.GetZ() - GetZ()*otherVector.GetY(), GetZ()*otherVector.GetX() - GetX()*otherVector.GetZ(), GetX()*otherVector.GetY() - GetY()*otherVector.GetX());
	return crossProduct;
}

Vector3 Vector3::ElementWisePow(const float i_exponent) const
{
	Vector3 result;
	result.SetParameters(std::pow(abs(GetX()), i_exponent), std::pow(abs(GetY()), i_exponent), std::pow(abs(GetZ()), i_exponent));
	result = result.GetMagnitude() * GetUnitVector();
	return result;
}

float Vector3::GetMagnitude() const
{
	float magnitude = sqrt(pow(GetX(), 2) + pow(GetY(), 2) + pow(GetZ(), 2));
	return magnitude;
}

Vector3 Vector3::GetUnitVector() const
{
	float magnitude = GetMagnitude();
	
	Vector3 unitVector(GetX() / magnitude, GetY() / magnitude, GetZ() / magnitude);
	
	if (magnitude == 0)
		return Vector3();

	return unitVector;
}

Vector3 operator*(const int & i_Scalar, const Vector3 & i_Vector)
{
	Vector3 product;
	product.SetParameters(i_Vector.GetX() * i_Scalar, i_Vector.GetY() * i_Scalar, i_Vector.GetZ() * i_Scalar);
	return product;
}

Vector3 operator*(const float & i_Scalar, const Vector3 & i_Vector)
{
	Vector3 product;
	product.SetParameters(i_Vector.GetX() * i_Scalar, i_Vector.GetY() * i_Scalar, i_Vector.GetZ() * i_Scalar);
	return product;
}