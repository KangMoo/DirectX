#pragma once
class Window
{
public:
	HRESULT CreateDxWindow(HINSTANCE hIns, int cmdShow);
	HRESULT CreateDevice(); //

public:
	Window();
	~Window();

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

};

