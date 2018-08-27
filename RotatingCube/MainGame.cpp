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
	D3DXVECTOR3 temp[8];

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			for (int k = 0; k < 2; k++)
			{
				temp[i + j * 2 + k * 4] = D3DXVECTOR3(k * 2 - 1, j * 2 - 1, i * 2 - 1);
			}
		}
	}


	DWORD color[8];
	color[0] = 0xFFF00000;
	color[1] = 0xFFF0F000;
	color[2] = 0xFFF00F00;
	color[3] = 0xFFF000F0;
	color[4] = 0xFFF0000F;
	color[5] = 0xFFF0FF00;
	color[6] = 0xFFF000FF;
	color[7] = 0xFFF00FF0;

	//좌표노가다&색깔~
	vertex[0][0].position = temp[2];
	vertex[0][1].position = temp[0];
	vertex[0][2].position = temp[1];
	vertex[0][3].position = temp[2];
	vertex[0][4].position = temp[1];
	vertex[0][5].position = temp[3];

	vertex[1][0].position = temp[6];
	vertex[1][1].position = temp[4];
	vertex[1][2].position = temp[0];
	vertex[1][3].position = temp[6];
	vertex[1][4].position = temp[0];
	vertex[1][5].position = temp[2];

	vertex[2][0].position = temp[7];
	vertex[2][1].position = temp[5];
	vertex[2][2].position = temp[4];
	vertex[2][3].position = temp[7];
	vertex[2][4].position = temp[4];
	vertex[2][5].position = temp[6];

	vertex[3][0].position = temp[3];
	vertex[3][1].position = temp[1];
	vertex[3][2].position = temp[5];
	vertex[3][3].position = temp[3];
	vertex[3][4].position = temp[5];
	vertex[3][5].position = temp[7];

	vertex[4][0].position = temp[0];
	vertex[4][1].position = temp[4];
	vertex[4][2].position = temp[5];
	vertex[4][3].position = temp[0];
	vertex[4][4].position = temp[5];
	vertex[4][5].position = temp[1];

	vertex[5][0].position = temp[6];
	vertex[5][1].position = temp[2];
	vertex[5][2].position = temp[3];
	vertex[5][3].position = temp[6];
	vertex[5][4].position = temp[3];
	vertex[5][5].position = temp[7];

	//~좌표노가다

	//색깔 노가다~
	vertex[0][0].color = color[2];
	vertex[0][1].color = color[0];
	vertex[0][2].color = color[1];
	vertex[0][3].color = color[2];
	vertex[0][4].color = color[1];
	vertex[0][5].color = color[3];

	vertex[1][0].color = color[6];
	vertex[1][1].color = color[4];
	vertex[1][2].color = color[0];
	vertex[1][3].color = color[6];
	vertex[1][4].color = color[0];
	vertex[1][5].color = color[2];

	vertex[2][0].color = color[7];
	vertex[2][1].color = color[5];
	vertex[2][2].color = color[4];
	vertex[2][3].color = color[7];
	vertex[2][4].color = color[4];
	vertex[2][5].color = color[6];

	vertex[3][0].color = color[3];
	vertex[3][1].color = color[1];
	vertex[3][2].color = color[5];
	vertex[3][3].color = color[3];
	vertex[3][4].color = color[5];
	vertex[3][5].color = color[7];

	vertex[4][0].color = color[0];
	vertex[4][1].color = color[4];
	vertex[4][2].color = color[5];
	vertex[4][3].color = color[0];
	vertex[4][4].color = color[5];
	vertex[4][5].color = color[1];

	vertex[5][0].color = color[6];
	vertex[5][1].color = color[2];
	vertex[5][2].color = color[3];
	vertex[5][3].color = color[6];
	vertex[5][4].color = color[3];
	vertex[5][5].color = color[7];

	//~색깔노가다
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
	return S_OK;
}

void MainGame::Release()
{

}

void MainGame::Update()
{
	angle += 1.0f;

	D3DXMATRIX matRotY;
	D3DXMATRIX matRotX;
	D3DXMATRIX matRotZ;
	//D3DXMatrixRotation~ : 회전함수
	D3DXMatrixRotationY(&matRotY, D3DXToRadian(angle));
	D3DXMatrixRotationX(&matRotX, D3DXToRadian(45));
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(45));
	D3DXMATRIX matTrans;
	//D3DXMatrixTranslation : 물체 이동 함수
	D3DXMatrixTranslation(&matTrans, 0, 0, 0);

	D3DXMATRIX matScale;
	//D3DXMatrixScaling : 물체 크기 변경 함수
	D3DXMatrixScaling(&matScale, 1, 1, 1);

	//위치,회전 곱하여 matWorld에 적용
	//SRT 순으로 곱하여 작업한다 (scale, rotation, translation)
	matWorld = matScale * matRotX*matRotZ*matRotY * matTrans;
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
		2 * 6,					//도형의 갯수
		vertex,				//그릴 도형의 데이터
		sizeof(tagVertex)	//stride 구조체의 크기
	);
}
