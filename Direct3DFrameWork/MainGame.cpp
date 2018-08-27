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
	//삼각형을 그릴때는 오른쪽 방향으로 돌겠금
	vertex[0].position = D3DXVECTOR3(-1, -1, -1);
	vertex[1].position = D3DXVECTOR3(-1,  1, -1);
	vertex[2].position = D3DXVECTOR3( 1,  1, -1);
	vertex[3].position = D3DXVECTOR3( 1,  -1, -1);

	vertex[4].position = D3DXVECTOR3(-1, -1,  1);
	vertex[5].position = D3DXVECTOR3(-1,  1,  1);
	vertex[6].position = D3DXVECTOR3( 1,  1,  1);
	vertex[7].position = D3DXVECTOR3( 1,  -1,  1);

	vertex[0].color = 0xFFFFFF00;
	vertex[1].color = 0xFF00FF00;
	vertex[2].color = 0xFF0000FF;
	vertex[3].color = 0xFFFFFF00;
	vertex[4].color = 0xFF0000FF;
	vertex[5].color = 0xFF00FF00;
	vertex[6].color = 0xFF00FF00;
	vertex[7].color = 0xFFFFFF00;

	//vertexBuffer 설정 및 초기화
	D3D::Get()->GetDevice()->CreateVertexBuffer(
		sizeof(tagVertex) * 8 //저장항 데이터의 총 크기 
		, D3DUSAGE_DYNAMIC //유동적으로 데이터를 사용하겠다 (재초기화 가능하)
		, tagVertex::FVF
		, D3DPOOL_DEFAULT //데이터 저장 방식 
		, &vertexBuffer
		, NULL
	);
	//vertexBuffer안에 내용쓰기
	tagVertex* vertices;
	vertexBuffer->Lock(
		0	//내용작성할 시작지점(index번호)
		, sizeof(tagVertex) * 8 //사이즈 
		, (void**)&vertices		//데이터를 받을 변수 
		, NULL);
	{
		memcpy(vertices, vertex, sizeof(tagVertex) * 8);
	}
	vertexBuffer->Unlock();

	//인덱스 설정	
	DWORD indices[36] =
	{
		0,1,2,0,2,3,	//후면
		3,2,6,3,6,7,    //우측면
		7,6,5,7,5,4,	//정면
		4,5,1,4,1,0,	//좌측
		1,5,6,1,6,2,    //윗면
		4,0,3,4,3,7		//밑면
	};
	//indexBuffer 초기화
	D3D::Get()->GetDevice()->CreateIndexBuffer
	(
		sizeof(DWORD) * 36,
		D3DUSAGE_DYNAMIC,
		D3DFMT_INDEX32,
		D3DPOOL_DEFAULT,
		&indexBuffer,
		NULL
	);

	void* pIndices;
	indexBuffer->Lock(0, sizeof(DWORD) * 36, &pIndices, NULL);
	{
		memcpy(pIndices, indices, sizeof(DWORD) * 36);
	}
	indexBuffer->Unlock();
	D3D::Get()->GetDevice()->SetRenderState(
		D3DRS_LIGHTING, false
	);
	//int =0; float 0.0f, bool false, string  = ""

	D3DXMatrixIdentity(&matWorld); //초기화
	D3DXMatrixIdentity(&matView); //초기화
	D3DXMatrixIdentity(&matProjection); //초기화

	//view matrix 만드는 
	D3DXMatrixLookAtLH(
		&matView,	//반환 행렬
		new D3DXVECTOR3(2, 0, -10),
		new D3DXVECTOR3(0, 0, 0),
		new D3DXVECTOR3(0, 1, 0)
	);
	//projection Matrix 설정
	D3DXMatrixPerspectiveFovLH(
		&matProjection,
		D3DX_PI / 3.0f,
		(float)AppDesc.width / (float)AppDesc.height,
		0.1f,
		1000.0f
	);

	angle = 0.0f;

	//////////////////////////////////////////////////
	quat[0].position = D3DXVECTOR3(-1, -1, 0);
	quat[1].position = D3DXVECTOR3(-1, 1, 0);
	quat[2].position = D3DXVECTOR3(1, 1, 0);
	quat[3].position = D3DXVECTOR3(1, -1, 0);

	quat[0].uv = D3DXVECTOR2(0, 1);
	quat[1].uv = D3DXVECTOR2(0, 0.5);
	quat[2].uv = D3DXVECTOR2(1, 0.5);
	quat[3].uv = D3DXVECTOR2(1, 1);

	//텍스쳐 
	D3DXCreateTextureFromFile
	(
		D3D::Get()->GetDevice(),
		"download.jpg", &pTex
	);
	D3DXMatrixIdentity(&matQuatWorld);
	//index 
	quatIndex[0] = 0;
	quatIndex[1] = 1;
	quatIndex[2] = 2;

	quatIndex[3] = 0;
	quatIndex[4] = 2;
	quatIndex[5] = 3;

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
	angle += 3.0f;

	D3DXMATRIX matRotY;
	D3DXMatrixRotationY(&matRotY, D3DXToRadian(angle));
	
	D3DXMATRIX matRotZ;
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(45.0f));

	D3DXMATRIX matRotX;
	D3DXMatrixRotationX(&matRotX, D3DXToRadian(45.0f));

	matWorld = matRotX  * matRotZ * matRotY;

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
	//D3D::Get()->GetDevice()->DrawPrimitiveUP(
	//	D3DPT_TRIANGLELIST,	//그릴 도형
	//	2,					//삼각형의 갯수 
	//	vertex,				//그릴 도형의 데이터 
	//	sizeof(tagVertex)	//stride 구조체 크기
	//);
	//Up->Cpu방식 랜더링
	//Up(x) ->Gpu 방식 랜더링 

	//D3D::Get()->GetDevice()->DrawIndexedPrimitiveUP
	//(
	//	D3DPT_TRIANGLELIST		//그릴도형
	//	, 0						//Vertex의 시작 인덱스
	//	, 8						//Vertex 배열의 갯수
	//	, 12					//그릴 도형의 갯수 
	//	, index					//index 데이터 주소값
	//	, D3DFMT_INDEX32		//index 데이터 1개의 크기 
	//	, vertex				//vertex 데이터 주소값
	//	, sizeof(tagVertex)		//vertex 구조체 크기 
	//);

	D3D::Get()->GetDevice()->SetStreamSource(
		0,		//스트림 번호 0번째 자리 
		vertexBuffer,
		0,		//바이트 시작지점
		sizeof(tagVertex)
	);
	D3D::Get()->GetDevice()->SetIndices(indexBuffer);

	//D3DPT_TRIANGLELIST //0,1,2,3,4,5,  0,1,2  3,4,5 
	//D3DPT_TRIANGLELIP //0,1,2 , 1,2,3, 2,3,4, 4,5,6
	//D3DPT_TRIANGLEFAN//0,1,2, 2,3,4
	//D3D::Get()->GetDevice()->DrawIndexedPrimitive(
	//	D3DPT_TRIANGLELIST //그릴도형
	//	, 0				  // vertex시작
	//	, 0				  //vertex시작지점
	//	, 8
	//	, 0
	//	, 12
	//);
	//////////////////////////////////////////////////////////
	D3D::Get()->GetDevice()->SetTransform(D3DTS_WORLD, &matQuatWorld);

	D3D::Get()->GetDevice()->SetTexture(0, pTex);
	D3D::Get()->GetDevice()->SetFVF(tagVertex2::FVF);
	D3D::Get()->GetDevice()->DrawIndexedPrimitiveUP(
		D3DPT_TRIANGLELIST,
		0,
		4,
		2,
		quatIndex,
		D3DFMT_INDEX32,
		quat,
		sizeof(tagVertex2)
	);
}
