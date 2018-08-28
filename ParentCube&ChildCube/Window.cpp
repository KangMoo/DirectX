#include "stdafx.h"
#include "Window.h"

_AppDesc AppDesc;
D3DXVECTOR3 AppMouse;
HRESULT Window::CreateDxWindow(HINSTANCE hIns, int cmdShow)
{
	AppDesc.hInstance = hIns;

	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);					//클래스 사이즈 
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc = WndProc;							//메세지 처리 함수 등록
	wcex.cbClsExtra = 0;								//메모리 시작 부분 (일반 0)
	wcex.cbWndExtra = 0;								//메모리 시작 부분 (일반 0)
	wcex.hInstance = hIns;								//OS 버전 
	wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);	//실행파일 아이콘 이미지 
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);		//창 내부에서 사용할 커서 아이콘
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);	//창 배경 색상(더블버퍼 사용시 쓸모 없음?)
	wcex.lpszMenuName = AppDesc.ApplicationName;		//메뉴 이름 등록 
	wcex.lpszClassName = AppDesc.ApplicationName;		//클래스 이름 등록 
														//나중에 창 생성시 key 값으로 사용 
	wcex.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);	//미리보기 아이콘 이미지 

	RegisterClassEx(&wcex); //클래스 등록 

	//HWDN 창의 번호 
	//ex -> 확장버전 
	AppDesc.hWnd = CreateWindowEx
	(
		WS_EX_APPWINDOW,	//EX style 
		AppDesc.ApplicationName, //key (같은 이름으로 등록된 클래스를 불러 오겠다.)
		AppDesc.ApplicationName, //타이틀 이름 
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,	//창의 시작지점 x
		CW_USEDEFAULT,	//창의 시작지점 y
		CW_USEDEFAULT,  //창의 크기
		CW_USEDEFAULT,  //창의 높이
		nullptr,
		nullptr,
		hIns,
		this);

	if (!AppDesc.hWnd) return E_FAIL;

	//창을 생성 했으면 창의 크기를 재정의 

	RECT rc = { 0,0 , (LONG)AppDesc.width, (LONG)AppDesc.height };

	UINT centerX = (GetSystemMetrics(SM_CXSCREEN) - (UINT)AppDesc.width / 2);
	UINT centerY = (GetSystemMetrics(SM_CYSCREEN) - (UINT)AppDesc.height);
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	MoveWindow(AppDesc.hWnd, centerX, centerY,
		rc.right - rc.left,
		rc.bottom - rc.top, TRUE);
	ShowWindow(AppDesc.hWnd, cmdShow); //창띄우기
	UpdateWindow(AppDesc.hWnd); //포커싱

	return S_OK;
}

HRESULT Window::CreateDevice()
{
	if (FAILED(D3D::Get()->CreateDevice()))
		return E_FAIL;
	return S_OK;
}

Window::Window()
{
	AppDesc = {
		nullptr, nullptr,
		(LPSTR)("Direct3D9"),
		1024,768
	};
}


Window::~Window()
{
	DestroyWindow(AppDesc.hWnd);
	UnregisterClass(AppDesc.ApplicationName, AppDesc.hInstance);

}

LRESULT Window::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_MOUSEMOVE:
	{
		AppMouse.x = static_cast<float>(LOWORD(lParam));
		AppMouse.y = static_cast<float>(HIWORD(lParam));
		break;
	}
	case WM_CLOSE: case WM_QUIT:
		PostQuitMessage(0);
		break;

	}
	return DefWindowProc(hWnd, msg, wParam,lParam);
}
