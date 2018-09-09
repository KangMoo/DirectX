#pragma once
#include <SDKDDKVer.h>
//file Name stdafx.h

//사용되지 않는 헤더를 참조 하지 않는다.
#define WIN32_LEAN_AND_MEAN 

#include <Windows.h>

#include <stdlib.h>
#include <malloc.h>
#include <memory>
#include <iostream>

#include <string>
#include <vector>
#include <map>

#include <assert.h>

//DirectX sdk;
//#include "c:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include/d3d9.h"
#include <d3d9.h>
#include <d3dx9.h>

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

//ImGui Header
#include "./_ImGui/imgui.h"
#include "./_ImGui/imgui_impl_dx9.h"
#pragma comment (lib, "_ImGui/ImGuiDx9.lib")

//Dx9 Deivce Class 
#include "D3D.h"

#include "Keyboard.h"
#include "Font.h"
#include "Gizmo.h"

struct _AppDesc
{
	HINSTANCE hInstance;
	HWND hWnd;
	LPSTR ApplicationName;
	UINT width;
	UINT height;
};
extern _AppDesc AppDesc;

#define SAFE_DELETE(p) {if(p) {delete(p); (p) = NULL;}}
#define SAFE_DELETE_ARRAY(p) {if(p) {delete[] (p); (p) = NULL;}}
#define SAFE_RELEASE(p) {if(p) {(p)->Release(); (p) = NULL;}}

#define GET_SET(type, Name, param)\
type Get##Name()\
{\
	return param;\
}\
void Set##Name(type _param)\
{\
	param = _param;\
}
