#include <algorithm>
#include "Cheat.h"


Cheat::Cheat()
{
	// Base addresses for various different modules
	ServerBaseAddress = (uintptr_t)GetModuleHandle(L"server.dll");
	ClientBaseAddress = (uintptr_t)GetModuleHandle(L"client.dll");
	EngineBaseAddress = (uintptr_t)GetModuleHandle(L"engine.dll");
	// Pointers
	StaticViewMatrixAddress = (uintptr_t*)(EngineBaseAddress + 0x59CB98);
	StaticEntityListAddress = (uintptr_t*)(ClientBaseAddress + 0xC4D1EC);
	StaticLocalPlayerAddress = (uintptr_t*)(ClientBaseAddress + 0xC3F6C0);
	StaticPitchViewAngle = (float*)(EngineBaseAddress + 0x467474);
	StaticYawViewAngle = (float*)(EngineBaseAddress + 0x467478);


	//Redirecting output stream for our use

	/*
	// To Validate Entity Structures
	//uintptr_t DynamicLocalPlayerAddress = *StaticLocalPlayerAddress;
	//uintptr_t* CurrentVirtualTablePointer = (uintptr_t*)DynamicLocalPlayerAddress;
	//SessionVirtualTableAddress = *CurrentVirtualTablePointer;


	*/
}

void Cheat::Update() {
	// This is mainly so the menu can be launched before entering game

	//To Validate Entity Structures
	uintptr_t DynamicLocalPlayerAddress = *StaticLocalPlayerAddress;
	uintptr_t* CurrentVirtualTablePointer = (uintptr_t*)DynamicLocalPlayerAddress;
	SessionVirtualTableAddress = *CurrentVirtualTablePointer;

}

void Cheat::AimAt(int Button_Current) {
	// Array holds information about the players were attempting to aim at
	std::vector<AimbotPlayerInfo> PlayersAimbotInfo;

	// Global Variables that hold which player we selected as the best candidate to aim at
	bool LockedOn = false;
	uintptr_t* CurrentEntityPointer = nullptr;
	C_TFPlayer* CurrentPlayer = nullptr;


	while (true) {
		
		
		if ((Button_Current == 0 && GetAsyncKeyState(_Keys::RIGHTMOUSEBUTTON) < 0) || (Button_Current == 1 && GetAsyncKeyState(_Keys::LEFTMOUSEBUTTON) < 0) || (Button_Current == 2 && GetAsyncKeyState(_Keys::LEFTSHIFT) < 0) || (Button_Current == 3 && GetAsyncKeyState(_Keys::LEFTCTRL) < 0) || (Button_Current == 4 && GetAsyncKeyState(_Keys::F_KEY) < 0)) {
			// Player is still actively holding down the aimbot key
			Cheat::SetAimKey(true);
		}
		else if ((Button_Current == 0 && GetAsyncKeyState(_Keys::RIGHTMOUSEBUTTON) == 0) || (Button_Current == 1 && GetAsyncKeyState(_Keys::LEFTMOUSEBUTTON) == 0) || (Button_Current == 2 && GetAsyncKeyState(_Keys::LEFTSHIFT) == 0) || (Button_Current == 3 && GetAsyncKeyState(_Keys::LEFTCTRL) == 0) || (Button_Current == 4 && GetAsyncKeyState(_Keys::F_KEY) == 0)) {
			// Player released the aimbot key
			Cheat::SetAimKey(false);
		}



		if (Cheat::GetAimKeyStatus() == true) {
			if (LockedOn) {
				// Once locked on either wait till locked on player dies or player releases aimbot key

				if (!Cheat::IsBadReadPtr(CurrentEntityPointer) && (CurrentEntityPointer != 0x0)) {
					C_TFPlayer* LocalPlayer = (C_TFPlayer*)Cheat::GetLocalPlayer();
					// Making sure is valid #1
					uintptr_t CurrentEntityVTable = *CurrentEntityPointer;
					uintptr_t* CurrentEntityVTablePointer = (uintptr_t*)CurrentEntityVTable;

					if ((CurrentEntityVTablePointer != 0x0)) {
						// Making sure is valid #2

						if (*CurrentEntityVTablePointer == Cheat::GetSessionVirtualTable()) {

							uintptr_t CurrentEntityPointerDeref = *CurrentEntityPointer;
							CurrentPlayer = (C_TFPlayer*)CurrentEntityPointerDeref;

							if ((CurrentPlayer != 0x0) && (CurrentPlayer->IsDormant == 0) && (CurrentPlayer->Health > 0)) {
								// 1) Making sure is valid #3
								// 2) Making sure player is not dormant and is being replicated
								// 3) Making sure player is still alive

								// Calculating the angle I need which will result in me aiming at the head
								Vec3 LocalPlayerPosition;
								LocalPlayerPosition.x = LocalPlayer->Position.x;
								LocalPlayerPosition.y = LocalPlayer->Position.y - 50;
								LocalPlayerPosition.z = LocalPlayer->Position.z;

								Vec3 CurrentPlayerPosition;
								CurrentPlayerPosition.x = CurrentPlayer->Position.x;
								CurrentPlayerPosition.y = CurrentPlayer->Position.y - 50;
								CurrentPlayerPosition.z = CurrentPlayer->Position.z;



								Vec2 newAngle = CalcAngle(LocalPlayerPosition, CurrentPlayerPosition);

								float Time = 0;
								float Duration = (float)Cheat::GetAimSmoothing();

								Vec2 ViewAngles = Cheat::GetViewAngles();

								while (Time <= Duration) {
									float LerpForPitch = Lerp(ViewAngles.y, newAngle.y, Time / Duration);
									float LerpForYaw = Lerp(ViewAngles.x, newAngle.x, Time / Duration);

									Cheat::SetYawAngle(LerpForYaw);
									Cheat::SetPitchAngle(LerpForPitch);

									Time = Time + 0.1;
									Sleep(1);
								}



							}

						}

					}
				}


			}
			else if (!LockedOn) {
				// Find the best candidates to lock on to and add them to array above
				for (uintptr_t i = 1; i < Cheat::GetMaxPlayers(); ++i) {
					// Start at one because my player is always the first entity in the list which is replicated to the client
					C_TFPlayer* LocalPlayer = (C_TFPlayer*)Cheat::GetLocalPlayer();
					uintptr_t CurrentEntity = ((uintptr_t)Cheat::GetEntityList() + (0x10 * i)); // Every entity is 10 bytes away from each other
					CurrentEntityPointer = (uintptr_t*)CurrentEntity;

					if (!Cheat::IsBadReadPtr(CurrentEntityPointer) && (CurrentEntityPointer != 0x0)) {
						// Making sure is valid #1
						uintptr_t CurrentEntityVTable = *CurrentEntityPointer;
						uintptr_t* CurrentEntityVTablePointer = (uintptr_t*)CurrentEntityVTable;

						if ((CurrentEntityVTablePointer != 0x0)) {
							// Making sure is valid #2

							if (*CurrentEntityVTablePointer == Cheat::GetSessionVirtualTable()) {

								uintptr_t CurrentEntityPointerDeref = *CurrentEntityPointer;
								CurrentPlayer = (C_TFPlayer*)CurrentEntityPointerDeref;

								if (CurrentPlayer != 0x0) {
									// Making sure is valid #3

									if ((CurrentPlayer->IsDormant == 0)) {
										// Making sure player is not dormant and is being replicated

										if (CurrentPlayer->Team != LocalPlayer->Team) {

											Vec3 LocalPlayerPosition;
											LocalPlayerPosition.x = LocalPlayer->Position.x;
											LocalPlayerPosition.y = LocalPlayer->Position.y - 50;
											LocalPlayerPosition.z = LocalPlayer->Position.z;

											Vec3 CurrentPlayerPosition;
											CurrentPlayerPosition.x = CurrentPlayer->Position.x;
											CurrentPlayerPosition.y = CurrentPlayer->Position.y - 50;
											CurrentPlayerPosition.z = CurrentPlayer->Position.z;


											// Calculating the angle I need which will result in me aiming at the head
											Vec2 newAngle = CalcAngle(LocalPlayerPosition, CurrentPlayerPosition);


											// Calculating the distance between the my current view angle and the angle returned
											float newAngle2 = Distance(Cheat::GetViewAngles(), newAngle);


											// Calculating the distance between my world space position and another world space position
											float newPos = Distance(LocalPlayer->Position, CurrentPlayer->Position);


											// Less than set FOV
											std::cout << "Current Distance Between Angles : " << newAngle2 << " My Set FOV : " << Cheat::GetFov() << std::endl;
											if (newAngle2 < Cheat::GetFov()) {
												// Then we save that players aimbot angle information inside a structure and throw it in a table to compare later
												AimbotPlayerInfo EnemyPlayer{ newAngle.x, newAngle.y, newAngle2, newPos, CurrentEntityPointer, (uintptr_t)CurrentPlayer };
												PlayersAimbotInfo.push_back(EnemyPlayer);
											}
										}
									}
								}

							}

						}
					}

				}
				// End of for loop


				// Making sure array is bigger than one to avoid undefined behavior
				// This method of sorting has a fringe case when it's only two elements inside the array hence the two conditionals
				// This uses the bubble sort alogrithm to find the player closet to us and with smallest angle change

				if (PlayersAimbotInfo.size() == 1) {
					// No sorting required our function above only found one candidate for us to aim at

					// Setting global variables for currently locked on player
					CurrentEntityPointer = PlayersAimbotInfo[0].EntityPointerAddress;
					CurrentPlayer = (C_TFPlayer*)PlayersAimbotInfo[0].EntityAddress;

					LockedOn = true;

				}
				if (PlayersAimbotInfo.size() >= 2) {
					// Bubble Sort

					// Sorting Via Closet To LocalPlayer Position
					for (int i = 0; i < 1; ++i) {

						/* For every iteration the loop below will go through
						   The array and compare it against all of the other elements
						   If it finds one that's less it will switch the positions to start
						*/
						for (int j = 0; j < PlayersAimbotInfo.size(); ++j) {

							if (PlayersAimbotInfo[i].PositionDistance > PlayersAimbotInfo[j].PositionDistance) {
								std::swap(PlayersAimbotInfo[i], PlayersAimbotInfo[j]);
							}
							else {
								// Do Nothing
							}

						}

					}

					// Sorting Via Smallest Angle Movement
					for (int i = 0; i < 1; ++i) {
						for (int j = 0; j < PlayersAimbotInfo.size(); ++j) {
							if (PlayersAimbotInfo[i].AngleDistance > PlayersAimbotInfo[j].AngleDistance) {
								std::swap(PlayersAimbotInfo[i], PlayersAimbotInfo[j]);
							}
							else {
								// Do Nothing
							}
						}
					}





					CurrentEntityPointer = PlayersAimbotInfo[0].EntityPointerAddress;
					CurrentPlayer = (C_TFPlayer*)PlayersAimbotInfo[0].EntityAddress;

					LockedOn = true;


				}



			}
		}
		else if (Cheat::GetAimKeyStatus() == false) {
			// Completely stops aimbot and breaks out of loop
			break;
		}

	}
};


Vec3 Cheat::GetBoneMatrix(uintptr_t StartOfMatrix) {

	Vec3 BonePosition;
	uintptr_t BonePositionXAddress = (StartOfMatrix + 0x0C); // Arithmetic issues with pointers so i did it this way
	float* BonePositionXPtr = (float*)BonePositionXAddress;
	BonePosition.x = *BonePositionXPtr;

	uintptr_t BonePositionYAddress = (StartOfMatrix + 0x1C);
	float* BonePositionYPtr = (float*)BonePositionYAddress;
	BonePosition.y = *BonePositionYPtr;

	uintptr_t BonePositionZAddress = (StartOfMatrix + 0x2C);
	float* BonePositionZPtr = (float*)BonePositionZAddress;
	BonePosition.z = *BonePositionZPtr;

	return BonePosition;
}

void Cheat::ExitCheat(HMODULE hModule)
{
	//MenuHook.UnHook();

	//fclose(F);
	//FreeConsole();
	FreeLibraryAndExitThread(hModule, 0);
}
void Cheat::SetPitchAngle(float PitchAngle) {
	if (StaticPitchViewAngle != nullptr) {
		*StaticPitchViewAngle = PitchAngle;
	}
}
void Cheat::SetYawAngle(float YawAngle) {
	if (StaticYawViewAngle != nullptr) {
		*StaticYawViewAngle = YawAngle;
	}
}
void Cheat::SetAimKey(bool Status)
{
	IsHoldingAimKey = Status;
}
void Cheat::SetFOV(float Amount) {
	FOV = Amount;
}
void Cheat::SetAimSmoothing(float Amount)
{
	AimSmoothing = Amount;
}
float Cheat::GetAimSmoothing()
{
	return AimSmoothing;
}

uintptr_t Cheat::GetLocalPlayer() {
	return *StaticLocalPlayerAddress;
}
uintptr_t Cheat::GetSessionVirtualTable() {
	return SessionVirtualTableAddress;
}
uintptr_t* Cheat::GetEntityList() {
	return StaticEntityListAddress;
}
uintptr_t* Cheat::GetViewMatrix()
{
	return StaticViewMatrixAddress;
}
Vec2 Cheat::GetViewAngles() {
	Vec2 PlayerAngles{
		*StaticYawViewAngle,
		*StaticPitchViewAngle
	};
	return PlayerAngles;
}
int Cheat::GetFov() {
	return FOV;
}
int Cheat::GetMaxPlayers() {
	return MaxPlayers;
}
bool Cheat::GetAimKeyStatus()
{
	return IsHoldingAimKey;
}
bool Cheat::IsBadReadPtr(void* p)
{
	MEMORY_BASIC_INFORMATION mbi = { 0 };
	if (::VirtualQuery(p, &mbi, sizeof(mbi)))
	{
		DWORD mask = (PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY);
		bool b = !(mbi.Protect & mask);
		// check the page is not a guard page
		if (mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS)) b = true;

		return b;
	}
	return true;
}