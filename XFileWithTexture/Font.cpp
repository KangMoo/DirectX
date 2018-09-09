#include "stdafx.h"
#include "Font.h"

Font* Font::instance = NULL;

Font * Font::Get()
{
	if (instance == NULL)
		instance = new Font;
	return instance;
}

void Font::Destroy()
{
	if (instance != NULL)
		SAFE_DELETE(instance);
}

void Font::Init()
{
	HRESULT hr = D3DXCreateFont(
		DEVICE,		//Device
		15.f,		//글자의 가로크기
		0.f,		//글자의 세로크기
		FW_BOLD,	//글자의 두께
		1,			//MipMapFillter()
		FALSE,		//기울기
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,	//외각선 처리
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, //옵션
		"굴림",
		&pDefaultFont
	);
	//DEFAULT_PITCH -> 기본 정렬
	//FF_DONTCARE -> 글자 크기 상관없이 비율 맞춰 사용

	assert(SUCCEEDED(hr) && "Not Initialize Font");
}

void Font::Release()
{
	SAFE_RELEASE(pDefaultFont);
}

void Font::PrintText(std::string str, int x, int y, DWORD color)
{
	RECT rcText = { x,y,0,0 };
	pDefaultFont->DrawTextA
	(
		NULL,
		str.c_str(),	//출력할 문자열
		-1,				//문자열의 길이 (-1 전부)
		&rcText,		//출력할 영역 
		DT_LEFT | DT_NOCLIP,
		color
	);
	//DT_LEFT -> 왼쪽 정렬
	//DT_VCENTER
	//DT_NOCLIP-> 영역에 상관없이 출력하겠다.
}

Font::Font()
{
	this->Init();
}


Font::~Font()
{
	this->Release();
}
