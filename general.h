
#pragma once
#include <Windows.h>
#include <iostream>
#include <tuple>
#include <vector>
#include <cmath>
#include <optional>
#include <stdexcept>
#include <algorithm>
#include <winnt.h>
#include <d3d9.h>
#include <d3dcompiler.h>
#include <TlHelp32.h>
#include <fstream>
#include <cstdlib>
#include <dwmapi.h>
#include "./DirectX/d3dx9math.h"
#include "./imgui/imgui.h"
#include "./imgui/imgui_impl_dx9.h"
#include "./imgui/imgui_impl_win32.h"
#include <d3d9.h>
#include <d3dcompiler.h>

inline HWND gameHWND; 
inline ImFont* Font1, * Font2, * iconFont1, * FontAwsome, * SmallAwasome;
#define M_PI 3.14159265358979323846264338327950288419716939937510

inline int actor_count = NULL;
inline int ScreenHeight = NULL;
inline int ScreenWidth = NULL;
inline int ScreenLeft = NULL;
inline int ScreenRight = NULL;
inline int ScreenTop = NULL;
inline int ScreenBottom = NULL;

inline float ScreenCenterX = ScreenWidth / 2;
inline float ScreenCenterY = ScreenHeight / 2;


void MainRender();
bool DirectXInit();
void DrawMenu();
void SetupWindow();


namespace OverlayWindow
{
	inline WNDCLASSEX WindowClass;
	inline HWND Hwnd;
	inline LPCSTR Name;
};

namespace DirectX9Interface
{
	inline IDirect3D9Ex* Direct3D9 = NULL;
	inline IDirect3DDevice9Ex* pDevice = NULL;
	inline D3DPRESENT_PARAMETERS pParams = { NULL };
	inline MARGINS Margin = { -1 };
	inline MSG Message = { NULL };
};




inline extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
inline LRESULT CALLBACK WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(hWnd, Message, wParam, lParam))
	{
		return true;
	}


	ImGuiIO& io = ImGui::GetIO();


	switch (Message) {
	case WM_DESTROY:
		if (DirectX9Interface::pDevice != NULL) {
			DirectX9Interface::pDevice->EndScene();
			DirectX9Interface::pDevice->Release();
		}
		if (DirectX9Interface::Direct3D9 != NULL) {
			DirectX9Interface::Direct3D9->Release();
		}
		PostQuitMessage(0);
		//exit(4);
		break;
	case WM_SIZE:
		if (DirectX9Interface::pDevice != NULL && wParam != SIZE_MINIMIZED) {
			ImGui_ImplDX9_InvalidateDeviceObjects();
			DirectX9Interface::pParams.BackBufferWidth = LOWORD(lParam);
			DirectX9Interface::pParams.BackBufferHeight = HIWORD(lParam);
			HRESULT hr = DirectX9Interface::pDevice->Reset(&DirectX9Interface::pParams);
			if (hr == D3DERR_INVALIDCALL)
				IM_ASSERT(0);
			ImGui_ImplDX9_CreateDeviceObjects();
		}
		break;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU)
			return 0;
		break;
	default:
		return DefWindowProc(hWnd, Message, wParam, lParam);
		break;
	}
	return 0;
}



namespace librarys
{
	inline HMODULE user32;
	inline HMODULE win32u;
	inline bool init()
	{

		HMODULE user32_lib = LoadLibraryA("user32.dll");
		if (!user32_lib) return false;
		HMODULE win32u_lib = LoadLibraryA("win32u.dll");
		if (!win32u_lib) return false;
		user32 = GetModuleHandleA("user32.dll");
		if (!user32) return false;
		win32u = GetModuleHandleA("win32u.dll");
		if (!win32u) return false;
		return true;
	}
}
inline PDIRECT3DTEXTURE9 my_texture = NULL;

inline int my_image_width = 120;
inline int my_image_height = 120;

namespace input
{
	static BYTE ntusersendinput_bytes[30] = { 0 };

	static BOOLEAN WINAPI init()
	{
		LPVOID ntusersendinput_addr = GetProcAddress(librarys::user32, "NtUserSendInput");
		if (!ntusersendinput_addr)
		{
			ntusersendinput_addr = GetProcAddress(librarys::win32u, "NtUserSendInput");
			if (!ntusersendinput_addr) return FALSE;
		}

		// Ensure function address is readable before copying
		if (IsBadReadPtr(ntusersendinput_addr, 30)) return FALSE;

		memcpy(ntusersendinput_bytes, ntusersendinput_addr, 30);
		return TRUE;
	}

	static BOOLEAN WINAPI ntusersendinput(UINT cinputs, LPINPUT pinputs, int cbsize)
	{
		// Validate input parameters
		if (cinputs == 0 || !pinputs || cbsize != sizeof(INPUT)) return FALSE;

		// Allocate executable memory safely
		LPVOID ntusersendinput_spoof = VirtualAlloc(nullptr, 0x1000, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		if (!ntusersendinput_spoof) return FALSE;

		// Ensure the memory is writable before copying
		if (IsBadWritePtr(ntusersendinput_spoof, 30))
		{
			VirtualFree(ntusersendinput_spoof, 0, MEM_RELEASE);
			return FALSE;
		}

		memcpy(ntusersendinput_spoof, ntusersendinput_bytes, 30);

		// Ensure the function pointer is valid before executing
		auto func_ptr = reinterpret_cast<NTSTATUS(NTAPI*)(UINT, LPINPUT, int)>(ntusersendinput_spoof);
		if (IsBadCodePtr(reinterpret_cast<FARPROC>(func_ptr)))
		{
			VirtualFree(ntusersendinput_spoof, 0, MEM_RELEASE);
			return FALSE;
		}

		NTSTATUS result = func_ptr(cinputs, pinputs, cbsize);

		// Securely clear allocated memory before releasing
		ZeroMemory(ntusersendinput_spoof, 0x1000);
		VirtualFree(ntusersendinput_spoof, 0, MEM_RELEASE);

		return (result > 0);
	}

	static BOOLEAN WINAPI move_mouse(int x, int y)
	{
		INPUT input = { 0 };
		input.type = INPUT_MOUSE;
		input.mi.mouseData = 0;
		input.mi.time = 0;
		input.mi.dx = x;
		input.mi.dy = y;
		input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_VIRTUALDESK;

		return ntusersendinput(1, &input, sizeof(input));
	}
}


static const char* keyNames[] =
{
	"",
	"Left Mouse",
	"Right Mouse",
	"Cancel",
	"Middle Mouse",
	"Mouse 5",
	"Mouse 4",
	"",
	"Backspace",
	"Tab",
	"",
	"",
	"Clear",
	"Enter",
	"",
	"",
	"Shift",
	"Control",
	"Alt",
	"Pause",
	"Caps",
	"",
	"",
	"",
	"",
	"",
	"",
	"Escape",
	"",
	"",
	"",
	"",
	"Space",
	"Page Up",
	"Page Down",
	"End",
	"Home",
	"Left",
	"Up",
	"Right",
	"Down",
	"",
	"",
	"",
	"Print",
	"Insert",
	"Delete",
	"",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
	"",
	"",
	"",
	"",
	"",
	"Numpad 0",
	"Numpad 1",
	"Numpad 2",
	"Numpad 3",
	"Numpad 4",
	"Numpad 5",
	"Numpad 6",
	"Numpad 7",
	"Numpad 8",
	"Numpad 9",
	"Multiply",
	"Add",
	"",
	"Subtract",
	"Decimal",
	"Divide",
	"F1",
	"F2",
	"F3",
	"F4",
	"F5",
	"F6",
	"F7",
	"F8",
	"F9",
	"F10",
	"F11",
	"F12",
};

