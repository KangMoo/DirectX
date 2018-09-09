#include "stdafx.h"
#include "RTT.h"


RTT::RTT()
{
}

RTT::RTT(int width, int height)
{
	//RenderTaget용 빈 Texture
	DEVICE->CreateTexture(
		width,
		height,
		1,
		D3DUSAGE_RENDERTARGET,	//랜더 타겟용 Texture
		D3DFMT_A8R8G8B8,		//ARGB포맷
		D3DPOOL_DEFAULT,		//랜더 타겟용은 무조건 디폴트로
		&pRenderTexture,		//주소값 저장
		NULL
	);
	//랜더 할 표면값
	DEVICE->CreateDepthStencilSurface(
		width,height,
		D3DFMT_D24S8,			//포맷의 깊이값 : 24, 스탠실 : 8
		D3DMULTISAMPLE_NONE,	//멀티샘플링 사용 X (여러장 만들지 않겠다)
		0,						//퀄리티 0
		TRUE,					//버퍼 교체 시 이전 버퍼 내용을 지울 것인지 여부
		&pRenderSurface,
		NULL
	);
}


RTT::~RTT()
{
	SAFE_RELEASE(pRenderSurface);
	SAFE_RELEASE(pRenderTexture);
}

void RTT::BeginDraw()
{

	DEVICE->GetRenderTarget(0, &pDeviceTargetSurface);
	DEVICE->GetDepthStencilSurface(&pDeviceDepthAndStencilSurface);

	//RenderTexture의 Surface를 얻는다.
	LPDIRECT3DSURFACE9 texSurface = NULL;
	if (SUCCEEDED(this->pRenderTexture->GetSurfaceLevel(0, &texSurface)))
	{
		//Texture 표면을 DEVICE의 target버퍼로 세팅
		DEVICE->SetRenderTarget(0, texSurface);
		//세팅된 surface 정보는 바로 날려주는 예의가 필요하다
		SAFE_RELEASE(texSurface);
	}
	DEVICE->SetDepthStencilSurface(pRenderSurface);
	DEVICE->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		0x0FFFFFFF, 1.0f, 0);

	//랜더
}

void RTT::EndDraw()
{
	DEVICE->SetRenderTarget(0, pDeviceTargetSurface);
	DEVICE->SetDepthStencilSurface(pDeviceDepthAndStencilSurface);

	SAFE_RELEASE(pDeviceTargetSurface);
	SAFE_RELEASE(pDeviceDepthAndStencilSurface);
}
