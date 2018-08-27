#pragma once
//½Ì±ÛÅæ ÆÐÅÏ 
//ÅÛÇÃ¸´ Å¬·¡½º¸¦ »ó¼Ó ¹ÞÀº ½Ì±ÛÅæ -> ÅÛÇÃ¸´ ½Ì±ÛÅæ
//´ÙÀÌ³ª¹Í ½Ì±ÛÅæ 
class D3D
{
private:
	D3D() {};
	~D3D()
	{
		pDevice->Release();
		pDevice = nullptr;
	}
	static D3D* instance;

	LPDIRECT3DDEVICE9 pDevice;

public:
	HRESULT CreateDevice();
	void BeginDraw();
	void EndDraw();

	LPDIRECT3DDEVICE9 GetDevice() { return pDevice; }

public:

	static D3D* Get()
	{
		if (instance == NULL)
			instance = new D3D;

		return instance;
	}
	static void Destroy()
	{
		if (instance != NULL)
		{
			delete instance;
			instance = NULL;
		}
	}
};

