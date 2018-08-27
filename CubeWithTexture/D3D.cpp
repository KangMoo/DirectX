#include "stdafx.h"
#include "D3D.h"

D3D* D3D::instance = NULL;

HRESULT D3D::CreateDevice()
{
	//Device 생성하려면 IDirect3d9객체가 필요
	LPDIRECT3D9 pD3D9;
	pD3D9 = Direct3DCreate9(D3D_SDK_VERSION);
	if (pD3D9 == NULL) return E_FAIL;

	//그래픽 카드 정보를 가지고 오는 녀석
	//그래픽 부스터 
	//버텍스 프로레싱 -> 
	D3DCAPS9 caps;
	pD3D9->GetDeviceCaps(
		D3DADAPTER_DEFAULT, //현재 화면을 출력하고 있는 그래픽카드를 사용
		D3DDEVTYPE_HAL,
		&caps);
	//Direct3D -> 화면출력시 특정 순서대로 작업을 하게 되어있음 (랜더링 파이프라인, 고정 파이프라인 이라고 함)
	//VtProccessing : 기본 9개 중 '라이트'처리 부분을 어떻게 처리할 것인지 (하드웨어처리가 속도가 더 빠름)
	DWORD VtProccessing = 0;
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) //하드웨어로 처리하는 상황이면 하드웨어로 처리/ else 소프트웨어로 처리
		VtProccessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else VtProccessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	D3DPRESENT_PARAMETERS d3dpp;
	d3dpp.BackBufferWidth = AppDesc.width;		//백버퍼 너비
	d3dpp.BackBufferHeight = AppDesc.height;	//백버퍼 높이
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;	//백버퍼 색상 포맷
	d3dpp.BackBufferCount = 1;					//백버퍼 갯수
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;//멀티샘플 사용여부 -> D3DMULTISAMPLE_NONE : 안티알리아싱 사용X
	d3dpp.MultiSampleQuality = 0;				//멀티샘플 사용하여 퀄리티 여부 0~15 범위 중 사용가능 -> 0 : 멀티샘플 사용X
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	//화면 전환시 데이터 처리
	d3dpp.hDeviceWindow = AppDesc.hWnd;			//띄울 창
	d3dpp.Windowed = true;						//화면에 띄울 윈도우 창이 존재하는지 여부 -> true : 창모드, false : 전체화면
	d3dpp.EnableAutoDepthStencil = true;		//
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;//포맷 설정 Depth : 24, Stencil : 8
	d3dpp.Flags = 0;							//옵션
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// d3dpp.Flags 옵션 종류																								 //
	// D3DPRESENTFLAG_LOCKABLE_BACKBUFFER : 후면버퍼를 잠그겠다 -> 잠글 수 있는 후면 버퍼는 성능 저하에 직접적 요인이 된다.	 //
	// D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL : 다음 후면 버퍼 사용 후 깊이 스텐실 데이터를 버리겠다. -> 성능향상에 도움이 된다.  //
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	//주사율 처리. D3DPRESENT_RATE_DEFAULT : 화면의 주사율과 랜더되는 주사율을 맞추겠다
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	//화면 전송간격 처리.D3DPRESENT_INTERVAL_DEFAULT : 화면 전송각격을 화면과 맞추겠다
	HRESULT result = pD3D9->CreateDevice(
		D3DADAPTER_DEFAULT,	//만들어질 Device객체와 대응될 물리 디스플레이 어댑터
		D3DDEVTYPE_HAL,		//이용할 장치 타입 지정. 하드웨어는 그래픽부스터 사용가능 소프트웨어는 사용 불가능. _HAL : 하드웨어, _REF : 소프트웨어
		AppDesc.hWnd,		//장치와 연결될 윈도우 핸들
		VtProccessing,		//정점(Vertex) 처리 방식
		&d3dpp,
		&pDevice);
	pD3D9->Release();
	pD3D9 = nullptr;

	return result;
}

void D3D::BeginDraw()
{
	//1.백버퍼 청소
	HRESULT hr = pDevice->Clear(
		0,							//청소할 영영의 d3dRect배열 갯수. 0 : 전체 클리어
		NULL,						//청소할 영역. NULL : 전체 클리어
		D3DCLEAR_STENCIL |			//스텥실값
		D3DCLEAR_TARGET |			//컬러값
		D3DCLEAR_ZBUFFER,			//깊이값
		D3DCOLOR_XRGB(25,25,25),	//청소 후 컬러값
		1.0f,						//청소 후 깊이값(0 ~ 1) 카메라를 기준으로 
		0.0f						//청소 후 스텐실값
		);

	//2. 백버퍼에 그리기
	if (SUCCEEDED(hr)) pDevice->BeginScene();
}

void D3D::EndDraw()
{
	pDevice->EndScene();						//다 그렸다
	pDevice->Present(NULL, NULL, NULL, NULL);	//백버퍼에 그린 내용을 전방버퍼로 전달
}
