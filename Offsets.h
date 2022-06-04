#pragma once
#include <Windows.h>
#include "Math.h"

class C_TFPlayer
{
public:
	char pad_0000[144]; //0x0000
	int32_t ClassID; //0x0090
	char pad_0094[16]; //0x0094
	int32_t IsDeadOrAlive; //0x00A4
	int32_t Health; //0x00A8
	char pad_00AC[88]; //0x00AC
	float HeadHeight; //0x0104
	char pad_0108[24]; //0x0108
	Vec3 Velocity; //0x0120
	char pad_012C[124]; //0x012C
	int32_t IsDormant; //0x01A8
	char pad_01AC[224]; //0x01AC
	Vec3 Position; //0x028C
	Vec2 ViewAngles; //0x0298
	char pad_02A0[148]; //0x02A0
	ViewMatrix_T ViewMatrix; //0x0334
	char pad_0374[484]; //0x0374
	int32_t Team; //0x0558
	char pad_055C[1828]; //0x055C
	int32_t MagAmmo; //0x0C80
	char pad_0C84[1036]; //0x0C84
};


