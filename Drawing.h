#pragma once
/*
This header file includes Direct3D9 things and dear ImGui things.
*/


#include <d3d9.h>
#include <d3dx9.h>
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"



typedef HRESULT(APIENTRY* tEndScene)(LPDIRECT3DDEVICE9 pDevice);
namespace _Direct3D9 {
	// Current Window Related Variables
	HWND Window;
	int WindowWidth;
	int WindowHeight;
	// Direct3D Variables
	ID3DXLine* PtrToLineInterface;

	struct Info {
		void* d3d9Device[119];
		tEndScene OriginalEndScene = nullptr;
		LPDIRECT3DDEVICE9 pD3DDevice = nullptr;
	};
	



	// Dummy Device Method Credit To Brohion and Rake @GuidedHacking
	BOOL CALLBACK EnumWindowsCallback(HWND handle, LPARAM lParam)
	{
		DWORD wndProcId;
		GetWindowThreadProcessId(handle, &wndProcId);

		if (GetCurrentProcessId() != wndProcId)
			return TRUE; // skip to next window

		_Direct3D9::Window = handle;
		return FALSE; // window found abort search
	}
	HWND GetProcessWindow()
	{
		_Direct3D9::Window = NULL;
		EnumWindows(EnumWindowsCallback, NULL);

		RECT SizeOfWindow;
		GetWindowRect(_Direct3D9::Window, &SizeOfWindow);
		WindowWidth = SizeOfWindow.right - SizeOfWindow.left;
		WindowHeight = SizeOfWindow.bottom - SizeOfWindow.top;


		return _Direct3D9::Window;
	}
	bool GetD3D9Device(void** pTable, size_t Size)
	{
		if (!pTable)
			return false;

		IDirect3D9* pD3D = Direct3DCreate9(D3D_SDK_VERSION);

		if (!pD3D)
			return false;

		IDirect3DDevice9* pDummyDevice = NULL;

		// options to create dummy device
		D3DPRESENT_PARAMETERS d3dpp = {};
		d3dpp.Windowed = false;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.hDeviceWindow = GetProcessWindow();

		HRESULT dummyDeviceCreated = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDummyDevice);

		if (dummyDeviceCreated != S_OK)
		{
			// may fail in windowed fullscreen mode, trying again with windowed mode
			d3dpp.Windowed = !d3dpp.Windowed;

			dummyDeviceCreated = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDummyDevice);

			if (dummyDeviceCreated != S_OK)
			{
				pD3D->Release();
				return false;
			}
		}

		memcpy(pTable, *reinterpret_cast<void***>(pDummyDevice), Size);

		pDummyDevice->Release();
		pD3D->Release();
		return true;
	}


	// Draw Functions
	

	void SlicedDrawEspBox2D(Vec2 TopPosition, Vec2 BottomPosition, IDirect3DDevice9* pDevice, int Thickness) {
		// Top and Bottom are both expected to be in screen space

		int HeightBetween = (TopPosition.y - BottomPosition.y);
		// Get height between the two positions in screen space

		// If you think about it were trying to draw a square
		// So we can factor in the height because all sides of square are equal
		// To sum up this whole part were basically making points in screen space
		// From our world space coordinates that we threw inside the W2S function

		Vec2 TopLeft;
		TopLeft.x = TopPosition.x - HeightBetween / 4;
		TopLeft.y = TopPosition.y;

		Vec2 TopLeft1;
		TopLeft1.x = TopPosition.x - HeightBetween / 10;
		TopLeft1.y = TopPosition.y;


		Vec2 TopRight;
		TopRight.x = TopPosition.x + HeightBetween / 4;
		TopRight.y = TopPosition.y;

		Vec2 TopRight1;
		TopRight1.x = TopPosition.x + HeightBetween / 10;
		TopRight1.y = TopPosition.y;



		Vec2 BottomLeft;
		BottomLeft.x = BottomPosition.x - HeightBetween / 4;
		BottomLeft.y = BottomPosition.y;

		Vec2 BottomLeft1;
		BottomLeft1.x = BottomPosition.x - HeightBetween / 10;
		BottomLeft1.y = BottomPosition.y;

		Vec2 BottomLeftMiddle;
		BottomLeftMiddle.x = BottomPosition.x - HeightBetween / 4;
		BottomLeftMiddle.y = BottomPosition.y + HeightBetween / 1.5;


		Vec2 BottomLeftMiddle2;
		BottomLeftMiddle2.x = BottomPosition.x - HeightBetween / 4;
		BottomLeftMiddle2.y = BottomPosition.y + HeightBetween / 3.5;

		Vec2 BottomRight;
		BottomRight.x = BottomPosition.x + HeightBetween / 4;
		BottomRight.y = BottomPosition.y;


		Vec2 BottomRight1;
		BottomRight1.x = BottomPosition.x + HeightBetween / 10;
		BottomRight1.y = BottomPosition.y;


		Vec2 BottomRightMiddle;
		BottomRightMiddle.x = BottomPosition.x + HeightBetween / 4;
		BottomRightMiddle.y = BottomPosition.y + HeightBetween / 1.5;


		Vec2 BottomRightMiddle2;
		BottomRightMiddle2.x = BottomPosition.x + HeightBetween / 4;
		BottomRightMiddle2.y = BottomPosition.y + HeightBetween / 3.5;


		if (PtrToLineInterface) {
			// Was having undefined behavior when attempting to put more than two
			// Vectors in one array not sure why figure it out later
			D3DXVECTOR2 TopHorizontalLine[2];
			D3DXVECTOR2 TopHorizontalLine2[2];



			D3DXVECTOR2 BottomHorizontalLine[2];
			D3DXVECTOR2 BottomHorizontalLine2[2];

			D3DXVECTOR2 LeftVerticalLine[2];
			D3DXVECTOR2 LeftVerticalLine2[2];


			D3DXVECTOR2 RightVerticalLine[2];
			D3DXVECTOR2 RightVerticalLine2[2];

			// Building Line Across
			// Top Horizontal
			TopHorizontalLine[0] = D3DXVECTOR2(TopLeft.x, TopLeft.y);
			TopHorizontalLine[1] = D3DXVECTOR2(TopLeft1.x, TopLeft1.y);

			TopHorizontalLine2[0] = D3DXVECTOR2(TopRight.x, TopRight.y);
			TopHorizontalLine2[1] = D3DXVECTOR2(TopRight1.x, TopRight1.y);

			// Bottom Horizontal
			BottomHorizontalLine[0] = D3DXVECTOR2(BottomLeft.x, BottomLeft.y);
			BottomHorizontalLine[1] = D3DXVECTOR2(BottomLeft1.x, BottomLeft1.y);

			BottomHorizontalLine2[0] = D3DXVECTOR2(BottomRight.x, BottomRight.y);
			BottomHorizontalLine2[1] = D3DXVECTOR2(BottomRight1.x, BottomRight1.y);

			// Building Line Vertically
			// Left
			LeftVerticalLine[0] = D3DXVECTOR2(TopLeft.x, TopLeft.y);
			LeftVerticalLine[1] = D3DXVECTOR2(BottomLeftMiddle.x, BottomLeftMiddle.y);


			//
			LeftVerticalLine2[0] = D3DXVECTOR2(BottomLeft.x, BottomLeft.y);
			LeftVerticalLine2[1] = D3DXVECTOR2(BottomLeftMiddle2.x, BottomLeftMiddle2.y);

			// Right
			RightVerticalLine[0] = D3DXVECTOR2(TopRight.x, TopRight.y);
			RightVerticalLine[1] = D3DXVECTOR2(BottomRightMiddle.x, BottomRightMiddle.y);

			RightVerticalLine2[0] = D3DXVECTOR2(BottomRight.x, BottomRight.y);
			RightVerticalLine2[1] = D3DXVECTOR2(BottomRightMiddle2.x, BottomRightMiddle2.y);


			PtrToLineInterface->SetWidth(Thickness);
			PtrToLineInterface->Draw(TopHorizontalLine, 2, D3DCOLOR_ARGB(255, 255, 0, 0));
			PtrToLineInterface->Draw(TopHorizontalLine2, 2, D3DCOLOR_ARGB(255, 255, 0, 0));

			PtrToLineInterface->Draw(BottomHorizontalLine, 2, D3DCOLOR_ARGB(255, 255, 0, 0));
			PtrToLineInterface->Draw(BottomHorizontalLine2, 2, D3DCOLOR_ARGB(255, 255, 0, 0));

			PtrToLineInterface->Draw(LeftVerticalLine, 2, D3DCOLOR_ARGB(255, 255, 0, 0));
			PtrToLineInterface->Draw(LeftVerticalLine2, 2, D3DCOLOR_ARGB(255, 255, 0, 0));

			PtrToLineInterface->Draw(RightVerticalLine, 2, D3DCOLOR_ARGB(255, 255, 0, 0));
			PtrToLineInterface->Draw(RightVerticalLine2, 2, D3DCOLOR_ARGB(255, 255, 0, 0));
		}
		else {
			// Create Line Interface
			D3DXCreateLine(pDevice, &PtrToLineInterface);
		}


















	}


	// Credit to esp1z1 from Guidedhacking
	void DrawLine2D(Vec2 Position, IDirect3DDevice9* pDevice, int Thickness) {

		if (Position.x < 0) return;

		if (PtrToLineInterface) {
			D3DXVECTOR2 Line[2];
			Line[0] = D3DXVECTOR2(Position.x, Position.y);
			Line[1] = D3DXVECTOR2(_Direct3D9::WindowWidth / 2, _Direct3D9::WindowHeight);
			PtrToLineInterface->SetWidth(Thickness);
			PtrToLineInterface->Draw(Line, 2, D3DCOLOR_ARGB(255, 255, 0, 0));
		}
		else {
			// Create Line Interface
			D3DXCreateLine(pDevice, &PtrToLineInterface);
		}

	}


}


extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
namespace _DearImGui {
	// I'd rather use a namespace than a class, it just causes a lot of problems down the line
	
	// Member Variables
	bool IsInitialized = false;
	WNDPROC oWndProc; // 
	WNDPROC OriginalWindowProc;
	ImVec2 SizeOfMenu{ 350, 250 };



	// Menu Variables
	bool ShowMenu = false;

	// Aimbot Info
	bool EnableAimbot = false;
	float AimSmoothing = 1.0f;
	float ThicknessOfEsp = 1.0f;
	float FOV = 5;
	const char* Buttons[] = { "RMB", "LMB", "SHIFT", "CTRL", "F" };
	int Button_Current = 0; // Based on what ever this value is will change the key for aimbot

	// Esp Info
	bool EnableEsp = false;
	bool ShowBox = false;
	bool ShowLines = false;
	bool ShowSkeleton = false;






	// Member Functions
	void InitializeImGui(LPDIRECT3DDEVICE9 pDevice) {
		// Setup Dear ImGui context
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();





		// Setup Platform/Renderer backends
		ImGui_ImplWin32_Init(_Direct3D9::Window);
		ImGui_ImplDX9_Init(pDevice);

		// Obviously so it's only intialized once in the endscene hook
		IsInitialized = true;
	}
	void UnHookImGui() {
		(WNDPROC)SetWindowLongPtr(_Direct3D9::Window, GWL_WNDPROC, (LONG_PTR)oWndProc);
		ImGui_ImplDX9_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		if (ShowMenu) {
			// If menu is shown then we allow messages sent from kernel/os to be handled by imgui version of wndproc
			// Basically like mouse input and other things
			ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
		}
		// We also return the original WndProc so the original window still works as intended
		return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
	}



}







