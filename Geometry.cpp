#include "Geometry.h"
#include <math.h>

#include <vector>


#define PI 3.1415927f



float Magnitude(Vec2 vec)
{
	return sqrtf(vec.x * vec.x + vec.y * vec.y);
}
float Magnitude(Vec3 vec)
{
	return sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}
float Distance(Vec2 LocalPlayerAngle, Vec2 EnemyPlayerAngle)
{
	Vec2 diff = LocalPlayerAngle - EnemyPlayerAngle;
	return Magnitude(diff);
}
float Distance(Vec3 LocalPlayerPos, Vec3 EnemyPlayerPos)
{
	Vec3 diff = LocalPlayerPos - EnemyPlayerPos;
	return Magnitude(diff);
}
float Lerp(float a, float b, float t) {
	return (a + (b - a) * t);
}
Vec2 CalcAngle(Vec3 LocalPlayerPosition, Vec3 EnemyPlayerPosition) {
	// Functions calculates an angle between two points in world space
	// Start out by subtracting the point we want to aim at by our point in world space
	// Which is defined as a vector3 in most cases
	Vec3 newPosition = EnemyPlayerPosition - LocalPlayerPosition;


	// To start off we use right triangles to

	// Using the pythagorean theorm we can find 

	float Z = sqrt(newPosition.x * newPosition.x + newPosition.y * newPosition.y + newPosition.z * newPosition.z);


	Vec2 newAngle{};

	newAngle.x = atan2(newPosition.y, newPosition.x) * (180 / PI); // We use atan to avoid have to do logic for different quadrants
	newAngle.y = -atan(newPosition.z / Z) * (180 / PI);

	return newAngle;
};
void WorldToScreen(Vec3 Position, Vec2& Screen, ViewMatrix_T Matrix, int Width, int Height) {
	// This function takes the Model View Projection matrix which has been multipied together and stored in memory.
	// An performs matrix multiplication to transform 3D world space positions to 2D screen space positions.

	
	Vec4 ClipSpaceCoordinates{
		Position.x* Matrix[0][0] + Position.y * Matrix[0][1] + Position.z * Matrix[0][2] + Matrix[0][3], // X
		Position.x* Matrix[1][0] + Position.y * Matrix[1][1] + Position.z * Matrix[1][2] + Matrix[1][3], // Y
		Position.x* Matrix[2][0] + Position.y * Matrix[2][1] + Position.z * Matrix[2][2] + Matrix[2][3], // Z
		Position.x* Matrix[3][0] + Position.y * Matrix[3][1] + Position.z * Matrix[3][2] + Matrix[3][3], // W
	};


	if (ClipSpaceCoordinates.w > 0.1f) {
		Vec3 NDC{
			ClipSpaceCoordinates.x / ClipSpaceCoordinates.w,
			ClipSpaceCoordinates.y / ClipSpaceCoordinates.w,
			ClipSpaceCoordinates.z / ClipSpaceCoordinates.w
		};

		Screen.x = (Width / 2 * NDC.x) + (NDC.x + Width / 2);
		Screen.y = -(Height / 2 * NDC.y) + (NDC.y + Height / 2);
	}


}


