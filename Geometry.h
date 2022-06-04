#pragma once
#include "Math.h"



float Distance(Vec3 src, Vec3 dst);
float Distance(Vec2 src, Vec2 dst);
float Lerp(float a, float b, float t);
Vec2 CalcAngle(Vec3, Vec3);
void WorldToScreen(Vec3 Position, Vec2& Screen, ViewMatrix_T Matrix, int Width, int Height);
