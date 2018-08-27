#pragma once
#include "Window.h"
class MainGame : public Window
{
	//vertex : 도형의 꼭지점을 
	//polygon : vertex 3개가 모여 1면을 
	struct tagVertex
	{
		D3DXVECTOR3 position; //무조건
		DWORD color; //색상 
		enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };
		//D3DFVF_XYZ -> 위치로
		//D3DFVF_DIFFUSE -> 컬러값 
		//D3DFVF_TEX1 -> 텍스쳐 좌표로 쓰겠다.
		//D3DFVF_NORMAL-> 노말 벡터 
		//D3DFVF_BINORMAL->
	};
	//index : vertex의 그리는 순서 


	struct tagVertex2
	{
		D3DXVECTOR3 position; //무조건 작성이 되어있어야 된다.
		D3DXVECTOR2 uv;

		enum{ FVF = D3DFVF_XYZ | D3DFVF_TEX1};
	};

	tagVertex2 quat[4];
	DWORD quatIndex[6];
	D3DXMATRIX  matQuatWorld;
	
	LPDIRECT3DTEXTURE9 pTex; //텍스쳐 저장하는 변수
	//Directx texture -> bmp, jpg, jng, raw, dds, jpeg, gif

	tagVertex vertex[8];
	DWORD index[36];

	LPDIRECT3DVERTEXBUFFER9 vertexBuffer;
	LPDIRECT3DINDEXBUFFER9 indexBuffer;

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
};

