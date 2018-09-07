#include "stdafx.h"
#include "RTT.h"


RTT::RTT()
{
}

RTT::RTT(int width, int height)
{
	//RenderTaget용 빈 Texture 만들기
	DEVICE->CreateTexture(
		width,
		height,
		1,
		D3DUSAGE_RENDERTARGET, //랜더 타겟용 Texture
		D3DFMT_A8R8G8B8,	//
		D3DPOOL_DEFAULT,    //랜더타겟용은 무조건 Default
		&pRenderTexture,    //주소값 저장
		NULL
	);

	//Render 할 Surface;
	DEVICE->CreateDepthStencilSurface
	(
		width, height,
		D3DFMT_D24FS8,
		D3DMULTISAMPLE_NONE,    //여러장 만들지 않겠다.
		0,						//퀄리티 0
		TRUE, //버퍼 교체시 이전 버퍼 내용을 지울 것이냐?
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
	//화면에 랜더링 되는 대신 Texture에 랜더한다.

	DEVICE->GetRenderTarget(0, &pDeviceTargetSurface);
	DEVICE->GetDepthStencilSurface(&pDeviceDepthAndStencilSurface);

	//RenderTexture의 Sufface를 얻는다.
	LPDIRECT3DSURFACE9 texSurface = NULL;
	if (SUCCEEDED(this->pRenderTexture->GetSurfaceLevel(0, &texSurface)))
	{
		//Texture 표면을 DEVICE의 target버퍼로 셋팅
		DEVICE->SetRenderTarget(0, texSurface);
		//셋팅된 Surface 정보는 바로 날려주는 예의가 필요
		SAFE_RELEASE(texSurface);
	}
	DEVICE->SetDepthStencilSurface(pRenderSurface);
	DEVICE->Clear(0, NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		0xFF00FF00, 1.0f, 0);
	//랜더 
}

void RTT::EndDraw()
{
	DEVICE->SetRenderTarget(0, pDeviceTargetSurface);
	DEVICE->SetDepthStencilSurface(pDeviceDepthAndStencilSurface);

	SAFE_RELEASE(pDeviceTargetSurface);
	SAFE_RELEASE(pDeviceDepthAndStencilSurface);
}
