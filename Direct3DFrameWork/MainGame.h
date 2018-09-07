#pragma once
#include "Window.h"
class MainGame : public Window
{
private:

	D3DXMATRIX matView;
	D3DXMATRIX matProjection;

	LPD3DXMESH pMeshSphere;
	D3DXMATRIX matSphere;

	LPD3DXEFFECT pEffect;

	struct tagVertex
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR4 color;
		enum {FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE};
	};
	tagVertex vertex[3];

	LPDIRECT3DTEXTURE9 pTex;
public:
	MainGame();
	~MainGame();

	HRESULT Init();
	void Release();
	void Update();
	void Render();

	void RenderTexutre();
	void GuiUpdate();

private:
	class DrawImGui* p;
	class RTT* rtt;
};

