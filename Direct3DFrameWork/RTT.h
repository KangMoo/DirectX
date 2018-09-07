#pragma once
//RenterTargetTexture
class RTT
{
private:
	int width;
	int height;

	LPDIRECT3DTEXTURE9 pRenderTexture;
	LPDIRECT3DSURFACE9 pRenderSurface;

	D3DXMATRIX matView;
	D3DXMATRIX matProjection;

	LPDIRECT3DSURFACE9 pDeviceTargetSurface;
	LPDIRECT3DSURFACE9 pDeviceDepthAndStencilSurface;

public:
	RTT();
	RTT(int width, int height);
	~RTT();

	void BeginDraw();
	void EndDraw();


	GET_SET(D3DXMATRIX, View, matView);
	GET_SET(D3DXMATRIX, Perspective, matProjection);
	LPDIRECT3DTEXTURE9 GetTexture() { return pRenderTexture; }
};

