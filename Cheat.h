#pragma once
#include <iostream>
#include <Windows.h>
#include <vector>

#include "Offsets.h"
#include "Geometry.h"
#include "Math.h"
#include "Input.h"



class Cheat
{
private:
	// Base Addresses
	uintptr_t ServerBaseAddress;
	uintptr_t EngineBaseAddress;
	uintptr_t ClientBaseAddress;
	uintptr_t SessionVirtualTableAddress;
	// Pointers
	uintptr_t* StaticEntityListAddress;
	uintptr_t* StaticLocalPlayerAddress;
	uintptr_t* StaticViewMatrixAddress;
	float* StaticPitchViewAngle;
	float* StaticYawViewAngle;
public:
	FILE* F = nullptr;


	// Other
	float AimSmoothing = 1;
	int FOV = 10;
	int MaxPlayers = 24;


	// Misc
	bool IsHoldingAimKey = false;


public:
	// Constructor
	Cheat();
	// Member functions
	void Update();
	void ExitCheat(HMODULE hModule);
	bool IsBadReadPtr(void* p);
	void AimAt(int CurrentButton);
	// Setters
	void SetPitchAngle(float PitchAngle);
	void SetYawAngle(float YawAngle);
	void SetAimKey(bool Status);
	void SetAimSmoothing(float Amount);
	void SetFOV(float Amount);
	// Getters
	float GetAimSmoothing();
	uintptr_t GetLocalPlayer();
	uintptr_t GetSessionVirtualTable();
	uintptr_t* GetEntityList();
	uintptr_t* GetViewMatrix();
	Vec2 GetViewAngles();
	Vec3 GetBoneMatrix(uintptr_t StartOfMatrix);
	int GetFov();
	int GetMaxPlayers();
	bool GetAimKeyStatus();
};
