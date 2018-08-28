#include "stdafx.h"
#include "D3D.h"

D3D* D3D::instance = NULL;

HRESULT D3D::CreateDevice()
{
	//Device 생성하려면 IDrectect3d9객체가 필요 
	LPDIRECT3D9 pD3D9;
	pD3D9 = Direct3DCreate9(D3D_SDK_VERSION);
	if (pD3D9 == NULL) return E_FAIL;

	//그래픽 카드를 정보를 가지고 오는 녀석
	//그래픽 부스터 
	//버텍스 프로레싱 -> 
	D3DCAPS9 caps;
	pD3D9->GetDeviceCaps(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		&caps);
	//Direct3D -> 화면출력시
	//랜더링 파이프 라인(고정 파이프 라인)
	//기본 9 중 라이트 
	DWORD VtProccessing = 0;
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		VtProccessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else VtProccessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	D3DPRESENT_PARAMETERS d3dpp;
	d3dpp.BackBufferWidth = AppDesc.width; //백버퍼 너비
	d3dpp.BackBufferHeight = AppDesc.height; //백버퍼 높이
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8; //백버퍼 색상 포맷
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE; //안티 알리아싱 사용하지 않겠다.
	d3dpp.MultiSampleQuality = 0; // 0~ 15
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; //화면 전환시 데이터 처리
	d3dpp.hDeviceWindow = AppDesc.hWnd; //띄울 창
	d3dpp.Windowed = true; //true 창모드 false 전체화면 
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.Flags = 0; //옵션

	//D3DPRESENTFLAG_LOCKABLE_BACKBUFFER
	//후면 버퍼를 잠그겠다 -> 잠글수 있는 후면 버퍼는 성능 저하에 직접적 요인이 된다.
	//D3DPRESENTFALG_DISCARD_DEPTHSTENCIL
	//다음 후면 버퍼를 사용후 깊이 스텐실 데이터를 버리겠다.
	//성능 향상에 도움이 된다.
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	//화면의 주사율과 랜더되는 주사율을 맞추겠다.
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	//화면 전송 간격 (화면과 맞추 겠다.)

	HRESULT result = pD3D9->CreateDevice(
		D3DADAPTER_DEFAULT, //만들어질 Device객체와 대응될 물리 디스플레이 어댑터 
		D3DDEVTYPE_HAL, // 이용할 장치 타입 지정 _REF (하드 웨더 -> 그래픽 부스터)
		AppDesc.hWnd, //장치와 연결될 윈도우 핸들
		VtProccessing,	//정점(Vertex) 처리방식
		&d3dpp,
		&pDevice);
	pD3D9->Release();
	pD3D9 = nullptr;

	return result;
}

void D3D::BeginDraw()
{
	HRESULT hr = pDevice->Clear(
		0, //청소할 영역의 d3dRect 배열 갯수(0 이면 전체 클리어)
		NULL, //청소할 영역 (NULL 이면 전체 클리어)
		D3DCLEAR_TARGET | //컬러 값
		D3DCLEAR_ZBUFFER | //깊이 값
		D3DCLEAR_STENCIL, //스텐실 값
		D3DCOLOR_XRGB(25, 25, 25), //청소후 컬러값
		1.0f, //청소 후 깊이값(0 ~ 1) 카메라를 기준으로 
		0.0f //청소후 스텐실값
	);
	//1. 백버퍼 청소 
	if (SUCCEEDED(hr)) pDevice->BeginScene();
	//2. 백퍼버에 그리겠다. 
}

void D3D::EndDraw()
{
	pDevice->EndScene(); //다 그렸다 
	//백버퍼에 그린 내용을 전방버퍼로 전달 
	pDevice->Present(NULL, NULL, NULL, NULL);
}
