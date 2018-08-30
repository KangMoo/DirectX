#pragma once
#include "Window.h"
class MainGame : public Window
{
	//struct tagVertex
	//{
	//	D3DXVECTOR3 position; //무조건 작성이 되어있어야 된다.
	//	D3DXVECTOR2 uv;
	//
	//	enum{ FVF = D3DFVF_XYZ | D3DFVF_TEX1};
	//};

	D3DMATERIAL9 material;
	D3DLIGHT9 light;

	LPDIRECT3DTEXTURE9 pTex; //텍스쳐 저장하는 변수
	//Directx texture -> bmp, jpg, jng, raw, dds, jpeg, gif

	LPDIRECT3DVERTEXBUFFER9 vertexBuffer;
	LPDIRECT3DINDEXBUFFER9 indexBuffer;

	D3DXMATRIX matWorld;
	D3DXMATRIX matView;
	D3DXMATRIX matProjection;
	D3DXMATRIX childWorld[4];

	float yAngle;
	float xAngle;
	float zAngle;
	D3DXVECTOR3 lightdirection;

	LPD3DXMESH mesh[4];
	D3DXVECTOR3 pos;
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

