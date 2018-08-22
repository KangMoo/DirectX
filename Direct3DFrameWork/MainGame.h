#pragma once
#include "Window.h"
class MainGame : public Window
{
	struct tagVertex
	{
		D3DXVECTOR3 position;
		DWORD color;
		enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };
	};
	tagVertex vertex[3];
public:
	MainGame();
	~MainGame();

	HRESULT Init();
	void Release();
	void Update();
	void Render();
};

