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
	D3DXVECTOR3 temp[8];
	temp[0] = D3DXVECTOR3(-1, -1, -1);
	temp[1] = D3DXVECTOR3(-1, 1, -1);
	temp[2] = D3DXVECTOR3(1, 1, -1);
	temp[3] = D3DXVECTOR3(1, -1, -1);
	temp[4] = D3DXVECTOR3(-1, -1, 1);
	temp[5] = D3DXVECTOR3(-1, 1, 1);
	temp[6] = D3DXVECTOR3(1, 1, 1);
	temp[7] = D3DXVECTOR3(1, -1, 1);


	//좌표노가다
	dice[0].position = temp[0];
	dice[1].position = temp[1];
	dice[2].position = temp[2];
	dice[3].position = temp[0];
	dice[4].position = temp[2];
	dice[5].position = temp[3];

	dice[6].position = temp[3];
	dice[7].position = temp[2];
	dice[8].position = temp[6];
	dice[9].position = temp[3];
	dice[10].position = temp[6];
	dice[11].position = temp[7];

	dice[12].position = temp[7];
	dice[13].position = temp[6];
	dice[14].position = temp[5];
	dice[15].position = temp[7];
	dice[16].position = temp[5];
	dice[17].position = temp[4];

	dice[18].position = temp[4];
	dice[19].position = temp[5];
	dice[20].position = temp[1];
	dice[21].position = temp[4];
	dice[22].position = temp[1];
	dice[23].position = temp[0];
	
	dice[24].position = temp[1];
	dice[25].position = temp[5];
	dice[26].position = temp[6];
	dice[27].position = temp[1];
	dice[28].position = temp[6];
	dice[29].position = temp[2];

	dice[30].position = temp[4];
	dice[31].position = temp[0];
	dice[32].position = temp[3];
	dice[33].position = temp[4];
	dice[34].position = temp[3];
	dice[35].position = temp[7];

	//좌표노가다
	dice[0].uv = D3DXVECTOR2(0.0f / 4.0f, 2.0f / 3.0f);
	dice[1].uv = D3DXVECTOR2(0.0f / 4.0f, 1.0f / 3.0f);
	dice[2].uv = D3DXVECTOR2(1.0f / 4.0f, 1.0f / 3.0f);
	dice[3].uv = D3DXVECTOR2(0.0f / 4.0f, 2.0f / 3.0f);
	dice[4].uv = D3DXVECTOR2(1.0f / 4.0f, 1.0f / 3.0f);
	dice[5].uv = D3DXVECTOR2(1.0f / 4.0f, 2.0f / 3.0f);

	dice[6].uv = D3DXVECTOR2(1.0f / 4.0f, 2.0f / 3.0f);
	dice[7].uv = D3DXVECTOR2(1.0f / 4.0f, 1.0f / 3.0f);
	dice[8].uv = D3DXVECTOR2(2.0f / 4.0f, 1.0f / 3.0f);
	dice[9].uv = D3DXVECTOR2(1.0f / 4.0f, 2.0f / 3.0f);
	dice[10].uv = D3DXVECTOR2(2.0f / 4.0f, 1.0f / 3.0f);
	dice[11].uv = D3DXVECTOR2(2.0f / 4.0f, 2.0f / 3.0f);

	dice[12].uv = D3DXVECTOR2(2.0f / 4.0f, 2.0f / 3.0f);
	dice[13].uv = D3DXVECTOR2(2.0f / 4.0f, 1.0f / 3.0f);
	dice[14].uv = D3DXVECTOR2(3.0f / 4.0f, 1.0f / 3.0f);
	dice[15].uv = D3DXVECTOR2(2.0f / 4.0f, 2.0f / 3.0f);
	dice[16].uv = D3DXVECTOR2(3.0f / 4.0f, 1.0f / 3.0f);
	dice[17].uv = D3DXVECTOR2(3.0f / 4.0f, 2.0f / 3.0f);

	dice[18].uv = D3DXVECTOR2(3.0f / 4.0f, 2.0f / 3.0f);
	dice[19].uv = D3DXVECTOR2(3.0f / 4.0f, 1.0f / 3.0f);
	dice[20].uv = D3DXVECTOR2(4.0f / 4.0f, 1.0f / 3.0f);
	dice[21].uv = D3DXVECTOR2(3.0f / 4.0f, 2.0f / 3.0f);
	dice[22].uv = D3DXVECTOR2(4.0f / 4.0f, 1.0f / 3.0f);
	dice[23].uv = D3DXVECTOR2(4.0f / 4.0f, 2.0f / 3.0f);

	dice[24].uv = D3DXVECTOR2(1.0f / 4.0f, 0.0f / 3.0f);
	dice[25].uv = D3DXVECTOR2(2.0f / 4.0f, 0.0f / 3.0f);
	dice[26].uv = D3DXVECTOR2(2.0f / 4.0f, 1.0f / 3.0f);
	dice[27].uv = D3DXVECTOR2(1.0f / 4.0f, 0.0f / 3.0f);
	dice[28].uv = D3DXVECTOR2(2.0f / 4.0f, 1.0f / 3.0f);
	dice[29].uv = D3DXVECTOR2(1.0f / 4.0f, 1.0f / 3.0f);

	dice[30].uv = D3DXVECTOR2(2.0f / 4.0f, 3.0f / 3.0f);
	dice[31].uv = D3DXVECTOR2(1.0f / 4.0f, 3.0f / 3.0f);
	dice[32].uv = D3DXVECTOR2(1.0f / 4.0f, 2.0f / 3.0f);
	dice[33].uv = D3DXVECTOR2(2.0f / 4.0f, 3.0f / 3.0f);
	dice[34].uv = D3DXVECTOR2(1.0f / 4.0f, 2.0f / 3.0f);
	dice[35].uv = D3DXVECTOR2(2.0f / 4.0f, 2.0f / 3.0f);


	D3D::Get()->GetDevice()->SetRenderState(
		D3DRS_LIGHTING, false
	);

	D3DXMatrixIdentity(&matWorld);		//초기화
	D3DXMatrixIdentity(&matView);		//초기화
	D3DXMatrixIdentity(&matProjection);	//초기화

	//view matrix 만드는 함수
	D3DXMatrixLookAtLH(
		&matView,	//반환행렬
		new D3DXVECTOR3(0, 0, -10),	//카메라의 위치 (eye)
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

	//D3DXMatrixIdentity(&mat)
	//텍스쳐
	D3DXCreateTextureFromFile(
		D3D::Get()->GetDevice(),	//디바이스
		"dice.png",				//이미지 파일 위치
		&pTex						//저장할 변수
	);
	D3DXMatrixIdentity(&matQuatWorld);
	return S_OK;
}

void MainGame::Release()
{
	vertexBuffer->Release();
	vertexBuffer = 0;
	indexBuffer->Release();
	indexBuffer = 0;
	pTex->Release();
	pTex = 0;
}

void MainGame::Update()
{
	angle += 1.0f;

	D3DXMATRIX matRotY;
	//D3DXMatrixRotation~ : 회전함수
	D3DXMatrixRotationY(&matRotY, D3DXToRadian(angle));

	D3DXMATRIX matRotZ;
	//D3DXMatrixRotation~ : 회전함수
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(45.0f));

	D3DXMATRIX matRotX;
	//D3DXMatrixRotation~ : 회전함수
	D3DXMatrixRotationX(&matRotX, D3DXToRadian(45.0f));

	matQuatWorld = matRotX * matRotZ *  matRotY;
}

void MainGame::Render()
{
	D3D::Get()->GetDevice()->SetTransform(
		D3DTS_VIEW, &matView);
	D3D::Get()->GetDevice()->SetTransform(
		D3DTS_PROJECTION, &matProjection);
	D3D::Get()->GetDevice()->SetFVF(tagVertex::FVF);


	D3D::Get()->GetDevice()->SetTransform(D3DTS_WORLD, &matQuatWorld);	//새로운 월드 설정

	D3D::Get()->GetDevice()->SetTexture(0, pTex);		//텍스쳐 설정.
	D3D::Get()->GetDevice()->SetFVF(tagVertex2::FVF);

	D3D::Get()->GetDevice()->DrawPrimitiveUP(
		D3DPT_TRIANGLELIST,	//그릴 도형
		12,					//도형의 갯수
		dice,				//그릴 도형의 데이터
		sizeof(tagVertex2)	//stride 구조체의 크기
	);
}
