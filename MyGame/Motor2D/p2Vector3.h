#ifndef __P2VECTOR3_H__
#define __P2VECTOR3_H__

#include <math.h>


template <class VALUE_TYPE> class Vector3
{
public:
	VALUE_TYPE x, y, z;

	//--------------------------------------------------------||
	//--------------------------------------------------------||
	Vector3(const VALUE_TYPE given_x, const VALUE_TYPE given_y, const VALUE_TYPE given_z)
	{
		x = given_x;
		y = given_y;
		z = given_z;
	}

	Vector3(const Vector3<VALUE_TYPE> given_vector)
	{
		x = given_vector.x;
		y = given_vector.y;
		z = given_vector.z;
	}
	
	Vector3() 
	{
		x = y = z = 0
	};
	//--------------------------------------------------------||
	//--------------------------------------------------------||
	bool operator==(const Vector3<VALUE_TYPE> given_vector) const
	{
		return(x == given_vector.x && y == given_vector.y && z == given_vector.z);
	}

	bool operator!=(const Vector3<VALUE_TYPE> given_vector) const
	{
		return(x != given_vector.x || y != given_vector.y || z != given_vector.z);
	}

	Vector3 operator+=(const Vector3<VALUE_TYPE> given_vector)
	{
		x += given_vector.x;
		y += given_vector.y;
		z += given_vector.z;

		return(this);
	}

	Vector3 operator-=(const Vector3<VALUE_TYPE> given_vector)
	{
		x -= given_vector.x;
		y -= given_vector.y;
		z -= given_vector.z;

		return(this);
	}

	Vector3 operator+(const Vector3<VALUE_TYPE> given_vector)
	{
		Vector3<VALUE_TYPE> resulting_vector;

		resulting_vector.x = x + given_vector.x;
		resulting_vector.y = y + given_vector.y;
		resulting_vector.z = z + given_vector.z;

		return(resulting_vector);
	}

	Vector3 operator-(const Vector3<VALUE_TYPE> given_vector)
	{
		Vector3<VALUE_TYPE> resulting_vector;

		resulting_vector.x = x - given_vector.x;
		resulting_vector.y = y - given_vector.y;
		resulting_vector.z = z - given_vector.z;

		return(resulting_vector);
	}
	//--------------------------------------------------------||
	//--------------------------------------------------------||
	VALUE_TYPE DistanceTo(const Vector3<VALUE_TYPE> vector_origin, const Vector3<VALUE_TYPE> vector_extreme) const
	{
		VALUE_TYPE distance;
		Vector3<VALUE_TYPE> distance_vector;

		distance_vector.x = vector_extreme.x - vector_origin.x;
		distance_vector.y = vector_extreme.y - vector_origin.y;
		distance_vector.z = vector_extreme.z - vector_origin.z;

		distance = sqrt((distance_vector.x*distance_vector.x) + (distance_vector.y*distance_vector.y) + (distance_vector.z*distance_vector.z));

		return(distance);
	}

	Vector3 Normalize()
	{
		TYPE_VALUE vectors_normal = sqrt((x*x)+(y*y)+(z*z));

		x = x / vectors_normal;
		y = y / vectors_normal;
		z = z / vectors_normal;

		return(this);
	}

	bool IsZero() const
	{
		return(x == 0 && y == 0 && z == 0);
	}

	Vector3 Zero()
	{
		x = y = z = 0;

		return(this);
	}
};


#endif