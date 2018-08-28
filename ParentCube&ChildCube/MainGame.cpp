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
	
	DEVICE->SetRenderState(D3DRS_LIGHTING, false);

	MakeCube();

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
	D3DXCreateTextureFromFile(
		DEVICE, "download.png", &pTex
	);

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

	
	D3DXMatrixTranslation(&matChild, 3, 0, 0);
	matChild *= matWorld;
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
	DEVICE->SetTexture(0, pTex);
	D3D::Get()->GetDevice()->SetStreamSource(
		0,		//스트림 번호 0번째 자리 
		vertexBuffer,
		0,		//바이트 시작지점
		sizeof(tagVertex)
	);
	D3D::Get()->GetDevice()->SetIndices(indexBuffer);

	D3D::Get()->GetDevice()->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST //그릴도형
		, 0				  // vertex시작
		, 0				  //vertex시작지점
		, 24
		, 0
		, 12
	);
	////////////////////////////////////////////////////////
	D3D::Get()->GetDevice()->SetTransform(
		D3DTS_WORLD, &matChild);
	D3D::Get()->GetDevice()->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST //그릴도형
		, 0				  // vertex시작
		, 0				  //vertex시작지점
		, 24
		, 0
		, 12
	);
}

void MainGame::MakeCube()
{
	tagVertex quad[4];
	
	quad[0].position = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	quad[1].position = D3DXVECTOR3(-1.0f, 1.0f, -1.0f);
	quad[2].position = D3DXVECTOR3(1.0f, 1.0f, -1.0f);
	quad[3].position = D3DXVECTOR3(1.0f, -1.0f, -1.0f);
	

	tagVertex cube[24];

	memcpy(cube, quad, sizeof(tagVertex) * 4);

	D3DXMATRIX matRot;
	D3DXMatrixRotationY(&matRot, D3DXToRadian(90.0f));
	for (int i = 0; i < 4; i++)
		D3DXVec3TransformCoord(&quad[i].position, &quad[i].position, &matRot);
	memcpy(cube + 4, quad, sizeof(tagVertex) * 4);

	D3DXMatrixRotationY(&matRot, D3DXToRadian(90.0f));
	for (int i = 0; i < 4; i++)
		D3DXVec3TransformCoord(&quad[i].position, &quad[i].position, &matRot);
	memcpy(cube + 8, quad, sizeof(tagVertex) * 4);

	D3DXMatrixRotationY(&matRot, D3DXToRadian(90.0f));
	for (int i = 0; i < 4; i++)
		D3DXVec3TransformCoord(&quad[i].position, &quad[i].position, &matRot);
	memcpy(cube + 12, quad, sizeof(tagVertex) * 4);

	D3DXMatrixRotationZ(&matRot, D3DXToRadian(90.0f));
	for (int i = 0; i < 4; i++)
		D3DXVec3TransformCoord(&quad[i].position, &quad[i].position, &matRot);
	memcpy(cube + 16, quad, sizeof(tagVertex) * 4);

	D3DXMatrixRotationZ(&matRot, D3DXToRadian(180.0f));
	for (int i = 0; i < 4; i++)
		D3DXVec3TransformCoord(&quad[i].position, &quad[i].position, &matRot);
	memcpy(cube + 20, quad, sizeof(tagVertex) * 4);


	for (int i = 0; i < 4; i++)
	{
		SetUV(cube + i * 4, D3DXVECTOR2(1.0f / 3.0f, i / 4.f));
	}
	SetUV(cube + 16, D3DXVECTOR2(0.0f / 3.0f, 1.f / 4.f));
	SetUV(cube + 20, D3DXVECTOR2(2.0f / 3.0f, 1.f / 4.f));

	DEVICE->CreateVertexBuffer(sizeof(tagVertex) * 24, D3DUSAGE_DYNAMIC, tagVertex::FVF, D3DPOOL_DEFAULT, &vertexBuffer, NULL);
	
	tagVertex* pVertices;
	vertexBuffer->Lock(0, sizeof(tagVertex) * 24, (void**)&pVertices, 0);
	{
		memcpy(pVertices, cube, sizeof(tagVertex) * 24);
	}
	vertexBuffer->Unlock();

	DWORD quadIndex[6] = { 0,1,2,0,2,3 };
	DWORD index[36];
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			index[6 * i + j] = quadIndex[j] + 4 * i;
		}
	}
	DEVICE->CreateIndexBuffer(sizeof(DWORD) * 36, D3DUSAGE_DYNAMIC, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &indexBuffer, NULL);
	void* pIndices;
	indexBuffer->Lock(0, sizeof(DWORD) * 36, &pIndices, 0);
	{
		memcpy(pIndices, index, sizeof(DWORD) * 36);
	}
	indexBuffer->Unlock();

}

void MainGame::SetUV(tagVertex * vertex, D3DXVECTOR2 offset)
{
	float IntervalU = 1.0f / 3.0f;
	float IntervalV = 1.0f / 4.0f;

	(vertex + 0)->uv = D3DXVECTOR2(offset.x, offset.y + IntervalV);
	(vertex + 1)->uv = D3DXVECTOR2(offset.x, offset.y);
	(vertex + 2)->uv = D3DXVECTOR2(offset.x + IntervalU, offset.y);
	(vertex + 3)->uv = D3DXVECTOR2(offset.x + IntervalU, offset.y + IntervalV);
}
