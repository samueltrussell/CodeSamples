//Member Declarations for Vector3 Class

#ifndef POSITIONVECTOR_H
#define POSITIONVECTOR_H

class Vector3
{
public:
	Vector3() : x(0), y(0), z(0) {};
	Vector3(float i_x, float i_y, float i_z);
	inline float const GetX() const;
	inline float const GetY() const;
	inline float const GetZ() const;
	inline void SetParameters(float i_x, float i_y, float i_z);
	inline void SetParameters(float i_x, float i_y);
	inline void SetX(float i_x);
	inline void SetY(float i_y);
	inline void SetZ(float i_z);
	inline Vector3 operator^(const float) const;
	void Print() const;
	bool operator==(const Vector3 &) const;
	void operator=(const Vector3 &);
	Vector3 operator+(const Vector3 &) const;
	void operator+=(const Vector3 &);
	Vector3 operator-(const Vector3 &) const;
	void operator-=(const Vector3 &);
	Vector3 operator*(const float &) const;
	float Dot(const Vector3 &) const;
	Vector3 Cross(const Vector3 &) const;
	Vector3 ElementWisePow(const float) const;
	float GetMagnitude() const;
	Vector3 GetUnitVector() const;
private:
	float x;
	float y;
	float z;

};

Vector3 operator*(const int & i_scalar, const Vector3 & i_Vector);
Vector3 operator*(const float & i_scalar, const Vector3 & i_Vector);

#include "Vector3-inl.h"

#endif