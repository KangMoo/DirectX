// ImGui Renderer for: DirectX9
// This needs to be used along with a Platform Binding (e.g. Win32)

// Implemented features:
//  [X] Renderer: User texture binding. Use 'LPDIRECT3DTEXTURE9' as ImTextureID. Read the FAQ about ImTextureID in imgui.cpp.

// You can copy and use unmodified imgui_impl_* files in your project. See main.cpp for an example of using this.
// If you use this binding you'll need to call 4 functions: ImGui_ImplXXXX_Init(), ImGui_ImplXXXX_NewFrame(), ImGui::Render() and ImGui_ImplXXXX_Shutdown().
// If you are new to ImGui, see examples/README.txt and documentation at the top of imgui.cpp.
// https://github.com/ocornut/imgui

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>

struct IDirect3DDevice9;

namespace ImGui {
	IMGUI_IMPL_API bool     ImplDX9_Init(IDirect3DDevice9* device);
	IMGUI_IMPL_API void     ImplDX9_Shutdown();
	IMGUI_IMPL_API void     ImplDX9_NewFrame();
	IMGUI_IMPL_API void     ImplDX9_RenderDrawData(ImDrawData* draw_data);

	// Use if you want to reset your rendering device without losing ImGui state.
	IMGUI_IMPL_API void     ImplDX9_InvalidateDeviceObjects();
	IMGUI_IMPL_API bool     ImplDX9_CreateDeviceObjects();

	// ImGui Platform Binding for: Windows (standard windows API for 32 and 64 bits applications)
	// This needs to be used along with a Renderer (e.g. DirectX11, OpenGL3, Vulkan..)

	// Implemented features:
	//  [X] Platform: Clipboard support (for Win32 this is actually part of core imgui)
	//  [X] Platform: Mouse cursor shape and visibility. Disable with 'io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange'.
	IMGUI_IMPL_API bool     ImplWin32_Init(void* hwnd);
	IMGUI_IMPL_API void     ImplWin32_Shutdown();
	IMGUI_IMPL_API void     ImplWin32_NewFrame();
	IMGUI_IMPL_API LRESULT  ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
}
