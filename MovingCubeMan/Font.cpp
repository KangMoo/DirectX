#include "stdafx.h"
#include "Font.h"

Font* Font::instance = NULL;

Font* Font::Get()
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
		DEVICE,							//Device
		15.f,							//글자의 가로크기
		0.f,							//글자의 세로크기
		FW_BOLD,						//글자의 두께
		1,								//MipMapFilter()
		FALSE,							//기울기 사용
		DEFAULT_CHARSET,				//영어사용
		OUT_DEFAULT_PRECIS,				//외곽선 처리
		DEFAULT_QUALITY,				//글자 퀄리티
		DEFAULT_PITCH | FF_DONTCARE,	//옵션 DEFAULT_PITCH : 기본정렬 / FF_DONTCARE : 글자 크기 상관 없이 비율 맞춰 사용
		"굴림",							//폰트
		&pDefaultFont					//저장
		);
	assert(SUCCEEDED(hr) && "Not Initialize Font");
}
void Font::PrintText(std::string str, int x, int y, DWORD color)
{
	RECT rcText = { x,y,0,0 };
	pDefaultFont->DrawTextA
	(
		NULL,
		str.c_str(),			//출력할 문자열
		-1,						//출력할 길이 -1 : 전부 다 출력
		&rcText,				//출력할 영역
		DT_LEFT | DT_NOCLIP,	//DT_LEFT : 왼쪽정렬 / DT_NOCLIP : 영역에 상관없이 출력하겠다
		color
	);
	//DT_VCENTER : 세로&가운데 정렬 (V:vertical)

}
void Font::Release()
{
	SAFE_RELEASE(pDefaultFont);
}

Font::Font()
{
	this->Init();
}


Font::~Font()
{
	this->Release();
}
