#pragma once
#include <Windows.h>
#include "Cheat.h"
#include "Drawing.h"
#include "Hook.h"
#include "Input.h"


// Global Objects
Cheat CheatInfo;
_Direct3D9::Info Direct3D9Info;
_Hook::TrampHook EndSceneHookInfo;

HRESULT APIENTRY hkEndScene(LPDIRECT3DDEVICE9 pDevice)
{

	Direct3D9Info.pD3DDevice = pDevice;

	if (!_DearImGui::IsInitialized) {
		_DearImGui::InitializeImGui(pDevice);
	}
	if (_DearImGui::ShowMenu) {
		{
			ImGui_ImplDX9_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();
			//ImGui::SetNextWindowFocus();

			{
				int No_ResizeFlag = 2;
				int No_WindowCollapseFlag = 32;

				ImGuiWindowFlags window_flags = No_ResizeFlag + No_WindowCollapseFlag;

				ImGui::SetNextWindowSize(_DearImGui::SizeOfMenu, 0);

				ImGui::Begin("Sparta's TF2 MultiHack", NULL, window_flags);


				if (ImGui::CollapsingHeader("Aimbot")) {

					ImGui::Checkbox("Enable Aimbot", &_DearImGui::EnableAimbot);




					ImGui::Text("Aimbot Smoothing");
					ImGui::SameLine();
					ImGui::SliderFloat("   ", &_DearImGui::AimSmoothing, 1.0f, 10.0f);

					ImGui::Text("Aimbot FOV");
					ImGui::SameLine();
					ImGui::SliderFloat("  ", &_DearImGui::FOV, 5.0f, 50.0f);


					ImGui::Text("Aimbot Key");
					ImGui::SameLine();
					ImGui::Combo("     ", &_DearImGui::Button_Current, _DearImGui::Buttons, IM_ARRAYSIZE(_DearImGui::Buttons));

				}
				if (ImGui::CollapsingHeader("Esp")) {
					ImGui::Checkbox("Enable ESP", &_DearImGui::EnableEsp);
					ImGui::Checkbox("Show Box", &_DearImGui::ShowBox);
					ImGui::Checkbox("Show Lines", &_DearImGui::ShowLines);
					//ImGui::Checkbox("Show Skeleton", &_DearImGui::ShowSkeleton);

					ImGui::Text("Thickness");
					ImGui::SameLine();
					ImGui::SliderFloat(" ", &_DearImGui::ThicknessOfEsp, 1.0f, 5.0f);
				}


				ImGui::End();
			}


			ImGui::EndFrame();
			ImGui::Render();
			ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		}
	}
	if (_DearImGui::EnableEsp) {
		if (CheatInfo.GetLocalPlayer() > 0) {
			// LocalPlayer is in-game
			for (uintptr_t i = 1; i < CheatInfo.GetMaxPlayers(); ++i) { // Start at one because my player is always the first entity in the list which is replicated to the client
				C_TFPlayer* LocalPlayer = (C_TFPlayer*)CheatInfo.GetLocalPlayer();
				uintptr_t CurrentEntity = ((uintptr_t)CheatInfo.GetEntityList() + (0x10 * i)); // Every entity is 10 bytes away from each other
				uintptr_t* CurrentEntityPointer = (uintptr_t*)CurrentEntity;

				if (!CheatInfo.IsBadReadPtr(CurrentEntityPointer) && (CurrentEntityPointer != 0x0)) {
					// Making sure is valid #1
					uintptr_t CurrentEntityVTable = *CurrentEntityPointer;
					uintptr_t* CurrentEntityVTablePointer = (uintptr_t*)CurrentEntityVTable;

					if ((CurrentEntityVTablePointer != 0x0)) {
						// Making sure is valid #2

						if (*CurrentEntityVTablePointer == CheatInfo.GetSessionVirtualTable()) {

							uintptr_t CurrentEntityPointerDeref = *CurrentEntityPointer;
							C_TFPlayer* CurrentPlayer = (C_TFPlayer*)CurrentEntityPointerDeref;

							if (CurrentPlayer != 0x0) {
								// Making sure is valid #3


								if ((CurrentPlayer->IsDormant == 0)) {
									// Making sure player is not dormant and is being replicated

									if ((CurrentPlayer->Team != LocalPlayer->Team) && (CurrentPlayer->Health > 0)) {
										// Color purposes
										D3DCOLOR CurrentPlayerBoxColor = D3DCOLOR_ARGB(255, 255, 0, 0);


										uintptr_t PointerToViewAValueInMatrix = (uintptr_t)CheatInfo.GetViewMatrix();
										uintptr_t ActualViewMatrix = (PointerToViewAValueInMatrix - 0x18); // Start of View Matrix


										ViewMatrix_T* PointerToViewMatrix = (ViewMatrix_T*)ActualViewMatrix;

										ViewMatrix_T ViewMatrix;
										memcpy(ViewMatrix, PointerToViewMatrix, sizeof(ViewMatrix_T));


										Vec2 ScreenCoordinates;
										Vec2 TopScreenCoordinates;

										Vec3 CurrPlayerPosition;
										CurrPlayerPosition.x = CurrentPlayer->Position.x;
										CurrPlayerPosition.y = CurrentPlayer->Position.y;
										CurrPlayerPosition.z = CurrentPlayer->Position.z;

										Vec3 CurrPlayerTopPosition;
										CurrPlayerTopPosition.x = CurrentPlayer->Position.x;
										CurrPlayerTopPosition.y = CurrentPlayer->Position.y;
										CurrPlayerTopPosition.z = CurrentPlayer->Position.z + 100;



										WorldToScreen(CurrPlayerPosition, ScreenCoordinates, ViewMatrix, _Direct3D9::WindowWidth, _Direct3D9::WindowHeight);
										WorldToScreen(CurrPlayerTopPosition, TopScreenCoordinates, ViewMatrix, _Direct3D9::WindowWidth, _Direct3D9::WindowHeight);

										if (_DearImGui::ShowBox) {
											_Direct3D9::SlicedDrawEspBox2D(TopScreenCoordinates, ScreenCoordinates, pDevice, _DearImGui::ThicknessOfEsp);
										}

										if (_DearImGui::ShowLines) {
											_Direct3D9::DrawLine2D(ScreenCoordinates, pDevice, _DearImGui::ThicknessOfEsp);
										}
									}




								}




							}
						}
					}
				}

			}
		}
	}

	/*
	{
		// Test Bone Matrix Esp
		uintptr_t BoneMatrixStartAddress = 0x572D0448;
		Vec2 ScreenPos;

		uintptr_t PointerToViewAValueInMatrix = (uintptr_t)CheatInfo.GetViewMatrix();
		uintptr_t ActualViewMatrix = (PointerToViewAValueInMatrix - 0x18); // Start of View Matrix


		ViewMatrix_T* PointerToViewMatrix = (ViewMatrix_T*)ActualViewMatrix;

		ViewMatrix_T ViewMatrix;
		memcpy(ViewMatrix, PointerToViewMatrix, sizeof(ViewMatrix_T));

		const char* lolbro = "L";

		for (int i = 1; i < 37; i++) {
			Vec3 BonePosition = CheatInfo.GetBoneMatrix(BoneMatrixStartAddress);
			WorldToScreen(BonePosition, ScreenPos, ViewMatrix, _Direct3D9::WindowWidth, _Direct3D9::WindowHeight);
			//_Direct3D9::DrawLine2D(ScreenPos, pDevice, 1);
			_Direct3D9::String(ScreenPos.x, ScreenPos.y, lolbro, D3DCOLOR_ARGB(255, 255, 0, 0), pDevice);
			BoneMatrixStartAddress = BoneMatrixStartAddress + (0x30 * i);


		}

	}
	*/
	// Returns and HRESULT from the function being called
	return Direct3D9Info.OriginalEndScene(pDevice);
}

void HookEndScene(_Hook::TrampHook& HookObject, _Direct3D9::Info& D3D9Object)
{
	// Hooking EndScene for drawing purposes
	if (_Direct3D9::GetD3D9Device(D3D9Object.d3d9Device, sizeof(D3D9Object.d3d9Device))) {
		HookObject.Hook((uintptr_t*)D3D9Object.d3d9Device[42], (uintptr_t*)hkEndScene, 7);
		D3D9Object.OriginalEndScene = (tEndScene)HookObject.GetGateway();
	};

}



void Run(HMODULE hModule) {
	HookEndScene(EndSceneHookInfo, Direct3D9Info);

	_DearImGui::oWndProc = (WNDPROC)SetWindowLongPtr(_Direct3D9::Window, GWL_WNDPROC, (LONG_PTR)_DearImGui::WndProc);


	//AllocConsole();
	//freopen_s(&CheatInfo.F, "CONOUT$", "w", stdout);

	while (true) {

		if (GetAsyncKeyState(_Keys::INSERT_KEY)) {
			if (!_DearImGui::ShowMenu) {
				_DearImGui::ShowMenu = true;
			}
			else {
				_DearImGui::ShowMenu = false;
			}
			Sleep(1000);
		}
		if (GetAsyncKeyState(_Keys::END_KEY)) {
			break;
		}

		if (CheatInfo.GetLocalPlayer() > 0) {
			CheatInfo.Update();
		}

		if (_DearImGui::EnableAimbot) {
			CheatInfo.SetFOV(_DearImGui::FOV);
			CheatInfo.SetAimSmoothing(_DearImGui::AimSmoothing);
			if ( (_DearImGui::Button_Current == 0 && GetAsyncKeyState(_Keys::RIGHTMOUSEBUTTON)) || (_DearImGui::Button_Current == 1 && GetAsyncKeyState(_Keys::LEFTMOUSEBUTTON)) || (_DearImGui::Button_Current == 2 && GetAsyncKeyState(_Keys::LEFTSHIFT)) || (_DearImGui::Button_Current == 3 && GetAsyncKeyState(_Keys::LEFTCTRL)) || (_DearImGui::Button_Current == 4 && GetAsyncKeyState(_Keys::F_KEY)) ) {
				if (CheatInfo.GetLocalPlayer() > 0) {
					// LocalPlayer is in-game
					CheatInfo.AimAt(_DearImGui::Button_Current);
				}
			}
		}



	}

	Sleep(1000);
	_DearImGui::UnHookImGui();
	EndSceneHookInfo.UnHook();
	CheatInfo.ExitCheat(hModule);
}
