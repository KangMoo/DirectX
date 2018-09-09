#pragma once

class Font
{
private:
	LPD3DXFONT pDefaultFont;
	static Font* instance;
public:
	static Font* Get();
	static void Destroy();

	void Init(); //폰트 생성
	void Release(); //폰트 삭제 

	void PrintText(std::string str, int x, int y, DWORD color);

private:
	Font();
	~Font();
};

#define FONT Font::Get()

