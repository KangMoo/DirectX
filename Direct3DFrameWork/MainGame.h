#pragma once
#include "Window.h"
class MainGame : public Window
{
private:

	D3DXMATRIX matView;
	D3DXMATRIX matProjection;

	LPD3DXMESH pMesh;
	D3DXMATRIX meshTrans;
	D3DXMATRIX meshLocal;

	DWORD dwMaterialNum;	//ºŒ¿Ã¥ı
	std::vector<LPDIRECT3DTEXTURE9> vecTextures;
	std::vector<D3DMATERIAL9> vecMaterials;	

	LPD3DXEFFECT pEffect;

public:
	MainGame();
	~MainGame();

	HRESULT Init();
	void Release();
	void Update();
	void Render();

	void RenderTexture();
	void GuiUpdate();

private:
	class DrawImGui* pDrawImGui;
	class RTT* rtt;
};

