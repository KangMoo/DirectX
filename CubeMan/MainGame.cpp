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

	tagVertex cube[24];
	MakeCube(cube, 1, 1.5, 0.5);
	tagVertex tempCube[24];
	memcpy(&tempCube, &cube, sizeof(tagVertex) * 24);
	for (int i = 0; i < 24; i++)
	{
		cube[i].position.y -= 1.5 / 2;
	}
	//////////////////
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
	//////////////////


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
	angleSwingD = FALSE;
	worldYAngle = 0.0f;
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
	if (angleSwingD)
	{
		angle += 2.0f;
	}
	else
	{
		angle -= 2.0f;
	}
	if (angle > 45)
	{
		angleSwingD = FALSE;
		angle = 45;
	}
	else if (angle < -45)
	{
		angleSwingD = TRUE;
		angle = -45;
	}

	worldYAngle += 0.5f;
	D3DXMATRIX matRotY;
	D3DXMatrixRotationY(&matRotY, D3DXToRadian(worldYAngle));

	D3DXMATRIX matRotZ;
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(45.0f));

	D3DXMATRIX matRotX;
	D3DXMatrixRotationX(&matRotX, D3DXToRadian(45.0f));


	D3DXMATRIX matScale;
	D3DXMatrixScaling(&matScale, 0.8, 0.8f / 1.5f, 0.8);
	D3DXMatrixTranslation(&matHead, 0, 1.0f / 1.5f, 0);
	matHead = matScale * matHead;
	D3DXMatrixScaling(&matScale, 0.5, 1.0f / 1.5f, 1);
	D3DXMatrixTranslation(&matLA, -0.75, -0.1, 0);
	D3DXMatrixTranslation(&matRA, 0.75, -0.1, 0);
	D3DXMatrixTranslation(&matLL, -0.25, -1.5, 0);
	D3DXMatrixTranslation(&matRL, 0.25, -1.5, 0);

	D3DXMATRIX matLRot;
	D3DXMatrixRotationX(&matLRot, D3DXToRadian(angle));
	D3DXMATRIX matRRot;
	D3DXMatrixRotationX(&matRRot, D3DXToRadian(-angle));
	matLA = matScale * matLRot* matLA;
	matRA = matScale * matRRot * matRA;
	matLL = matScale * matRRot * matLL;
	matRL = matScale * matLRot * matRL;

	matHead *= matWorld;
	matLA *= matWorld;
	matRA *= matWorld;
	matLL *= matWorld;
	matRL *= matWorld;
	matWorld = matRotY;
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
		D3DTS_WORLD, &matHead);
	D3D::Get()->GetDevice()->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST //그릴도형
		, 0				  // vertex시작
		, 0				  //vertex시작지점
		, 24
		, 0
		, 12
	);
	D3D::Get()->GetDevice()->SetTransform(
		D3DTS_WORLD, &matLA);
	D3D::Get()->GetDevice()->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST //그릴도형
		, 0				  // vertex시작
		, 0				  //vertex시작지점
		, 24
		, 0
		, 12
	);
	D3D::Get()->GetDevice()->SetTransform(
		D3DTS_WORLD, &matRA);
	D3D::Get()->GetDevice()->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST //그릴도형
		, 0				  // vertex시작
		, 0				  //vertex시작지점
		, 24
		, 0
		, 12
	);
	D3D::Get()->GetDevice()->SetTransform(
		D3DTS_WORLD, &matLL);
	D3D::Get()->GetDevice()->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST //그릴도형
		, 0				  // vertex시작
		, 0				  //vertex시작지점
		, 24
		, 0
		, 12
	);
	D3D::Get()->GetDevice()->SetTransform(
		D3DTS_WORLD, &matRL);
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


void MainGame::MakeCube(float xsize, float ysize, float zsize)
{
	tagVertex quad[8];
	quad[0].position = D3DXVECTOR3(-xsize / 2.0f, -ysize / 2.0f, -zsize / 2.0f);
	quad[1].position = D3DXVECTOR3(-xsize / 2.0f, ysize / 2.0f, -zsize / 2.0f);
	quad[2].position = D3DXVECTOR3(xsize / 2.0f, ysize / 2.0f, -zsize / 2.0f);
	quad[3].position = D3DXVECTOR3(xsize / 2.0f, -ysize / 2.0f, -zsize / 2.0f);
	quad[4].position = D3DXVECTOR3(-xsize / 2.0f, -ysize / 2.0f, zsize / 2.0f);
	quad[5].position = D3DXVECTOR3(-xsize / 2.0f, ysize / 2.0f, zsize / 2.0f);
	quad[6].position = D3DXVECTOR3(xsize / 2.0f, ysize / 2.0f, zsize / 2.0f);
	quad[7].position = D3DXVECTOR3(xsize / 2.0f, -ysize / 2.0f, zsize / 2.0f);

	tagVertex cube[24];
	D3DXMATRIX matRot;
	//전
	cube[0].position = quad[0].position;
	cube[1].position = quad[1].position;
	cube[2].position = quad[2].position;
	cube[3].position = quad[3].position;
	//우
	cube[4].position = quad[3].position;
	cube[5].position = quad[2].position;
	cube[6].position = quad[6].position;
	cube[7].position = quad[7].position;
	//후
	cube[8].position = quad[7].position;
	cube[9].position = quad[6].position;
	cube[10].position = quad[5].position;
	cube[11].position = quad[4].position;
	//좌
	cube[12].position = quad[4].position;
	cube[13].position = quad[5].position;
	cube[14].position = quad[1].position;
	cube[15].position = quad[0].position;
	//상
	cube[16].position = quad[5].position;
	cube[17].position = quad[6].position;
	cube[18].position = quad[2].position;
	cube[19].position = quad[1].position;
	//하
	cube[20].position = quad[7].position;
	cube[21].position = quad[4].position;
	cube[22].position = quad[0].position;
	cube[23].position = quad[3].position;


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

void MainGame::MakeCube(tagVertex* cube, float xsize, float ysize, float zsize)
{
	tagVertex quad[8];
	quad[0].position = D3DXVECTOR3(-xsize / 2.0f, -ysize / 2.0f, -zsize / 2.0f);
	quad[1].position = D3DXVECTOR3(-xsize / 2.0f, ysize / 2.0f, -zsize / 2.0f);
	quad[2].position = D3DXVECTOR3(xsize / 2.0f, ysize / 2.0f, -zsize / 2.0f);
	quad[3].position = D3DXVECTOR3(xsize / 2.0f, -ysize / 2.0f, -zsize / 2.0f);
	quad[4].position = D3DXVECTOR3(-xsize / 2.0f, -ysize / 2.0f, zsize / 2.0f);
	quad[5].position = D3DXVECTOR3(-xsize / 2.0f, ysize / 2.0f, zsize / 2.0f);
	quad[6].position = D3DXVECTOR3(xsize / 2.0f, ysize / 2.0f, zsize / 2.0f);
	quad[7].position = D3DXVECTOR3(xsize / 2.0f, -ysize / 2.0f, zsize / 2.0f);

	D3DXMATRIX matRot;
	//전
	cube[0].position = quad[0].position;
	cube[1].position = quad[1].position;
	cube[2].position = quad[2].position;
	cube[3].position = quad[3].position;
	//우
	cube[4].position = quad[3].position;
	cube[5].position = quad[2].position;
	cube[6].position = quad[6].position;
	cube[7].position = quad[7].position;
	//후
	cube[8].position = quad[7].position;
	cube[9].position = quad[6].position;
	cube[10].position = quad[5].position;
	cube[11].position = quad[4].position;
	//좌
	cube[12].position = quad[4].position;
	cube[13].position = quad[5].position;
	cube[14].position = quad[1].position;
	cube[15].position = quad[0].position;
	//상
	cube[16].position = quad[5].position;
	cube[17].position = quad[6].position;
	cube[18].position = quad[2].position;
	cube[19].position = quad[1].position;
	//하
	cube[20].position = quad[7].position;
	cube[21].position = quad[4].position;
	cube[22].position = quad[0].position;
	cube[23].position = quad[3].position;
}