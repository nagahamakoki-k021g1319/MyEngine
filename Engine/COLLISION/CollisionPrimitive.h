#pragma once
/// <summary>
///当たり判定プリミティブ
/// </summary>

#include"Vector3.h"
#include<DirectXMath.h>

struct Sphere {
	Vector3 center = { 0,0,0};

	float radius = 1.0f;

};

struct Plane {
	Vector3 normal = { 0,1,0 };

	float distance = 0.0f;

};

class Triangle {
public:
	Vector3 p0;
	Vector3 p1;
	Vector3 p2;

	Vector3 normal;

	void ComputeNormal();
};

struct Ray
{
	Vector3 start = { 0,0,0 };

	Vector3 dir = { 1,0,0 };

};

class CollisionPrimitive
{
	
};

