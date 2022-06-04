#pragma once
#include <Windows.h>


namespace _Hook {
	class TrampHook {
	private:
		uintptr_t* m_Source;
		uintptr_t* m_Destination;
		uintptr_t* m_GateWay;
		// For UnHooking Purposes
		uintptr_t* m_OriginalGateWayBytes; 
		uintptr_t* m_OriginalEndSceneBytes;


		int m_Length;
	public:
		// Constructor
		//TrampHook();

		// Member Functions
		void Hook(uintptr_t* Source, uintptr_t* Destination, int Length) {
			if (Length > 5) {


				m_Source = Source;
				m_Destination = Destination;
				m_Length = Length;

				// Allocate Memory For Stolen Bytes
				uintptr_t* Gateway = (uintptr_t*)VirtualAlloc(0, Length + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

				m_GateWay = Gateway;

				m_OriginalGateWayBytes = (uintptr_t*)VirtualAlloc(0, Length + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
				m_OriginalEndSceneBytes = (uintptr_t*)VirtualAlloc(0, Length + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);




				//std::cout << std::hex << m_OriginalEndSceneBytes << std::endl;
				//std::cout << std::hex << m_OriginalGateWayBytes << std::endl;


				// Save bytes in Gateway before copying stolen bytes
				memcpy(m_OriginalGateWayBytes, Gateway, Length);
				memcpy(m_OriginalEndSceneBytes, Source, Length);

				// Copy Stolen Bytes Into GateWay
				memcpy(Gateway, Source, Length);

				// Add a Relative Jump At The End Of GateWay Function
				uintptr_t GatewayAndLength = ((uintptr_t)Gateway + Length);
				*(uintptr_t*)GatewayAndLength = 0xE9;

				// Calculating The Address Back To The Hooked Function Five Bytes After We Hooked
				uintptr_t GateWayJumpBackAddress = ((uintptr_t)Source - (uintptr_t)Gateway - 5);

				//
				uintptr_t AddAddressAfterJump = ((uintptr_t)Gateway + Length + 1);

				*(uintptr_t*)AddAddressAfterJump = GateWayJumpBackAddress;




				DWORD CurrentProtection;
				VirtualProtect(Source, Length, PAGE_EXECUTE_READWRITE, &CurrentProtection);

				uintptr_t RelativeAddress = ((uintptr_t)Destination - (uintptr_t)Source - 5);
				*Source = 0xE9;
				uintptr_t AddAddressAfterJump2 = ((uintptr_t)Source + 1);
				*(uintptr_t*)AddAddressAfterJump2 = RelativeAddress;

				DWORD TempProtection;
				VirtualProtect(Source, Length, CurrentProtection, &TempProtection);


				m_GateWay = Gateway;

			}
		}
		void UnHook() {
			// Change Protections
			DWORD CurrentProtection;
			VirtualProtect(m_Source, m_Length, PAGE_EXECUTE_READWRITE, &CurrentProtection);
			DWORD CurrentProtection2;
			VirtualProtect(m_GateWay, m_Length + 5, PAGE_EXECUTE_READWRITE, &CurrentProtection2);





			memcpy(m_Source, m_OriginalEndSceneBytes, m_Length);
			memcpy(m_GateWay, m_OriginalGateWayBytes, m_Length + 5);




			DWORD TempProtection;
			VirtualProtect(m_Source, m_Length, CurrentProtection, &TempProtection);
			DWORD TempProtection2;
			VirtualProtect(m_GateWay, m_Length, CurrentProtection2, &TempProtection2);
		}


		// Getter
		uintptr_t* GetGateway() {
			return m_GateWay;
		}

	};
}



