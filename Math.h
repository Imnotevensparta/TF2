#pragma once
#include <iostream>

typedef float ViewMatrix_T[4][4];


struct Vec4 {
	float x, y, z, w;
};
struct Vec3 {
	float x, y, z;
	// Overloaded Operators
	friend Vec3 operator-(const Vec3& vec1, const Vec3& vec2) {
		Vec3 newPos;
		newPos.x = vec1.x - vec2.x;
		newPos.y = vec1.y - vec2.y;
		newPos.z = vec1.z - vec2.z;
		return newPos;
	}
	friend std::ostream& operator<<(std::ostream& out, const Vec3& vec) {
		out << "( " << vec.x << ", " << vec.z << ", " << vec.y << ") ";
		return out;
	}
};
struct Vec2 {
	float x, y;
	// Overloaded Operator
	friend Vec2 operator-(const Vec2& vec1, const Vec2& vec2) {
			Vec2 newPos;
			newPos.x = vec1.x - vec2.x;
			newPos.y = vec1.y - vec2.y;
			return newPos;
	}
};
struct AimbotPlayerInfo {
	float YawAngle;
	float PitchAngle;
	float AngleDistance;
	float PositionDistance;
	uintptr_t* EntityPointerAddress;
	uintptr_t EntityAddress;
};
