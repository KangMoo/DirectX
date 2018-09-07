
//main.cpp
#pragma once
#include "stdafx.h"
#include "MainGame.h"

MainGame* _mg;
int APIENTRY wWinMain( 
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow
)
{

	_mg = new MainGame;

	if (FAILED(_mg->CreateDxWindow(hInstance, nCmdShow))) return FALSE;
	if (FAILED(_mg->CreateDevice()))return FALSE;
	if (FAILED(_mg->Init())) return FALSE;
	Keyboard::Create();
	MSG msg;
	while(true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Input->Update();
			_mg->Update();
			{
				ImGui::ImplDX9_NewFrame();
				ImGui::ImplWin32_NewFrame();

				ImGui::NewFrame();
				//ImGui¿ë ÇÔ¼ö
				_mg->GuiUpdate();
				ImGui::EndFrame();
			}
			_mg->RenderTexture();

			D3D::Get()->BeginDraw();
			{
				_mg->Render();
				ImGui::Render();
				ImGui::ImplDX9_RenderDrawData(ImGui::GetDrawData());
			}
			D3D::Get()->EndDraw();
		}
	}
	FONT->Destroy();
	Keyboard::Delete();
	_mg->Release();
	delete _mg;

	return (int)msg.wParam;
}