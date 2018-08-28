#pragma once
#include "Window.h"
class MainGame : public Window
{
	struct tagVertex
	{
		D3DXVECTOR3 position; //무조건 작성이 되어있어야 된다.
		D3DXVECTOR2 uv;

		enum{ FVF = D3DFVF_XYZ | D3DFVF_TEX1};
	};

	
	LPDIRECT3DTEXTURE9 pTex; //텍스쳐 저장하는 변수
	//Directx texture -> bmp, jpg, jng, raw, dds, jpeg, gif

	LPDIRECT3DVERTEXBUFFER9 vertexBuffer;
	LPDIRECT3DINDEXBUFFER9 indexBuffer;

	D3DXMATRIX matChild;

	D3DXMATRIX matWorld;
	D3DXMATRIX matView;
	D3DXMATRIX matProjection;

	float angle;
public:
	MainGame();
	~MainGame();

	HRESULT Init();
	void Release();
	void Update();
	void Render();

	void MakeCube();
	void SetUV(tagVertex* vertex, D3DXVECTOR2 offset);
};

