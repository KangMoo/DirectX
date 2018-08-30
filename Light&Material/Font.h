#pragma once
#include <string>
class Font
{
private:
	LPD3DXFONT pDefaultFont;
	static Font* instance;

public:
	void Init();		//폰트 생성
	void Release();		//폰트 삭제
	static Font* Get();
	static void Destroy();

	void PrintText(std::string str, int x, int y, DWORD color);

private:
	Font();
	~Font();
};

#define FONT Font::Get()