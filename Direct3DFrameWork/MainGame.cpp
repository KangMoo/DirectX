#include "stdafx.h"
#include "MainGame.h"


MainGame::MainGame()
{
}


MainGame::~MainGame()
{
}

HRESULT MainGame::Init()
{
	//삼각형을 그릴때는 시계 방향으로 돌아가게끔 그려야 한다 (후면처리를 위해)
	//시계방향으로 그려야 정면처리가 됨
	vertex[0].position = D3DXVECTOR3(-1, -1, 0);
	vertex[1].position = D3DXVECTOR3(-1, 1, 0);
	vertex[2].position = D3DXVECTOR3(1, 1, 0);

	vertex[3].position = D3DXVECTOR3(-1, -1, 0);
	vertex[4].position = D3DXVECTOR3(1, 1, 0);
	vertex[5].position = D3DXVECTOR3(1, -1, 0);

	vertex[0].color = 0xFFFFFF00;
	vertex[1].color = 0xFF00FF00;
	vertex[2].color = 0xFF0000FF;

	vertex[3].color = 0xFFFFFF00;
	vertex[4].color = 0xFF0000FF;
	vertex[5].color = 0xFF00FF00;

	D3D::Get()->GetDevice()->SetRenderState(
		D3DRS_LIGHTING, false
	);

	D3DXMatrixIdentity(&matWorld);		//초기화
	D3DXMatrixIdentity(&matView);		//초기화
	D3DXMatrixIdentity(&matProjection);	//초기화

	//view matrix 만드는 함수
	D3DXMatrixLookAtLH(
		&matView,	//반환행렬
		new D3DXVECTOR3(0, 0, -5),	//카메라의 위치 (eye)
		new D3DXVECTOR3(0, 0, 0),	//바라보는 좌표 (at)
		new D3DXVECTOR3(0, 1, 0)	//카메라의 윗방향 (up)
	);

	//projection Matrix 설정
	D3DXMatrixPerspectiveFovLH(
		&matProjection,
		D3DX_PI / 3.0f,									//fov(라디안 값으로 넣어야 함)
		(float)AppDesc.width / (float)AppDesc.height,	//aspect
		0.1f,											//zn (렌더링 시작할 범위 지정)
		1000.0f											//zf (렌더링 끝낼 범위 지정)
	);
	angle = 0.0f;
	return S_OK;
}

void MainGame::Release()
{

}

void MainGame::Update()
{
	angle += 1.0f;

	D3DXMATRIX matRot;
	//D3DXMatrixRotation~ : 회전함수
	D3DXMatrixRotationY(&matRot, D3DXToRadian(angle));

	D3DXMATRIX matTrans;
	//D3DXMatrixTranslation : 물체 이동 함수
	D3DXMatrixTranslation(&matTrans, 2, 0, 0);

	D3DXMATRIX matScale;
	//D3DXMatrixScaling : 물체 크기 변경 함수
	D3DXMatrixScaling(&matScale, 2, 1, 1);
	
	//위치,회전 곱하여 matWorld에 적용
	//SRT 순으로 곱하여 작업한다 (scale, rotation, translation)
	matWorld = matScale * matRot * matTrans;
}

void MainGame::Render()
{
	D3D::Get()->GetDevice()->SetTransform(
		D3DTS_WORLD, &matWorld);
	D3D::Get()->GetDevice()->SetTransform(
		D3DTS_VIEW, &matView);
	D3D::Get()->GetDevice()->SetTransform(
		D3DTS_PROJECTION, &matProjection);
	D3D::Get()->GetDevice()->SetFVF(tagVertex::FVF);
	D3D::Get()->GetDevice()->DrawPrimitiveUP(
		D3DPT_TRIANGLELIST,	//그릴 도형
		2,					//도형의 갯수
		vertex,				//그릴 도형의 데이터
		sizeof(tagVertex)	//stride 구조체의 크기
	);
}
