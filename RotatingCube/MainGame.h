#pragma once
#include "Window.h"
class MainGame : public Window
{
	//vertex : 도형의 꼭지점
	//polygon : vertex 3개가 모여 만들어진 1개의 면
	struct tagVertex
	{
		D3DXVECTOR3 position;	//위치값 (필수)
		DWORD color;			//색상값 표현 (다른 텍스쳐로 대체 가능)
		enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };
		//D3DFVF_XYZ -> 위치로 사용하겠다
		//D3DFVF_DIFFUSE -> 컬러값으로 사용하겠다
		//D3DFVF_TEX1 -> 텍스쳐 좌표로 사용하겠다
		//D3DFVF_NORMAL -> 노말 벡터로 사용하겠다
		//D3DFVF_BINORMAL -> 바이노말로 사용하겠다
	};
	tagVertex vertex[6][6];
	tagVertex temp[6];
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

