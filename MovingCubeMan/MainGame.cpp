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
		new D3DXVECTOR3(2, 5, -20),
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

	pos = D3DXVECTOR3(0, 0, 0);

	angleSwingD = FALSE;
	headAngle = 0;
	headSwingD = FALSE;
	worldYAngle = 0;
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
	if (isCubemanMoving)
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
	}
	else
	{
		if (angle > 1)
			angle--;
		else if (angle < -1)
			angle++;
		else
			angle = 0;
	}


	if (headSwingD)
	{
		headAngle += 0.2f;
	}
	else
	{
		headAngle -= 0.2f;
	}
	if (headAngle > 15)
	{
		headSwingD = FALSE;
		headAngle = 15;
	}
	else if (headAngle < -15)
	{
		headSwingD = TRUE;
		headAngle = -15;
	}


	if (Input->Press(VK_UP) || Input->Press(VK_DOWN) || Input->Press(VK_LEFT) || Input->Press(VK_RIGHT))
	{
		isCubemanMoving = TRUE;
	}
	else
	{
		isCubemanMoving = FALSE;
	}
	int move = 0;
	if (Input->Down(VK_SPACE))
	{
		worldYAngle += 45;
	}
	if (Input->Press(VK_UP))
	{
		pos.z -= 0.1*cos(PI / 180 * worldYAngle);
		pos.x -= 0.1*sin(PI / 180 * worldYAngle);
	}
	if (Input->Press(VK_DOWN))
	{
		pos.z += 0.1*cos(PI / 180 * worldYAngle);
		pos.x += 0.1*sin(PI / 180 * worldYAngle);
	}
	if (Input->Press(VK_LEFT))
	{
		worldYAngle -= 1.5;
	}
	if (Input->Press(VK_RIGHT))
	{
		worldYAngle += 1.5;
	}

	//if(headAngle>15)
		

	D3DXMATRIX matTrans;
	D3DXMatrixTranslation(&matTrans, pos.x, float(5) / 2.0, pos.z);
	D3DXMATRIX matRotY;
	D3DXMatrixRotationY(&matRotY, D3DXToRadian(worldYAngle));

	D3DXMATRIX matRotZ;
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(0));

	D3DXMATRIX matRotX;
	D3DXMatrixRotationX(&matRotX, D3DXToRadian(0));

	//matWorld = matRotX  * matRotZ * matRotY * matTrans;

	D3DXMATRIX matScale;
	D3DXMatrixScaling(&matScale, 0.8, 0.8f / 1.5f, 0.8);
	D3DXMatrixTranslation(&matHead, 0, 1.0f / 1.5f, 0);

	D3DXMATRIX matHRot;
	D3DXMatrixRotationY(&matHRot, D3DXToRadian(headAngle));
	matHead = matScale * matHRot * matHead;
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
	matWorld = matRotX * matRotZ * matRotY * matTrans;

	//D3DXMatrixTranslation(&matChild, 3, 0, 0);
}
void MainGame::Render()
{
	GIZMO->WorldGrid(10,10);

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
		, 24			//
		, 0				//
		, 12			//
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
	FONT->PrintText("Font Test", 20, 40, 0xFFFFFFFF);
	GIZMO->DrawLine(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 1, 0), 0XFF00FFFF);
}

void MainGame::MakeCube()
{

	tagVertex body[24];
	UTIL->MakeCube(body, 1, 1.5, 0.5, D3DXVECTOR3(0, -1.5 / 2, 0));

	for (int i = 0; i < 4; i++)
	{
		SetUV(body + i * 4, D3DXVECTOR2(1.0f / 3.0f, i / 4.f));
	}
	SetUV(body + 16, D3DXVECTOR2(0.0f / 3.0f, 1.f / 4.f));
	SetUV(body + 20, D3DXVECTOR2(2.0f / 3.0f, 1.f / 4.f));

	DEVICE->CreateVertexBuffer(sizeof(tagVertex) * 24 * 6, D3DUSAGE_DYNAMIC, tagVertex::FVF, D3DPOOL_DEFAULT, &vertexBuffer, NULL);

	tagVertex* pVertices;
	vertexBuffer->Lock(0, sizeof(tagVertex) * 24 * 6, (void**)&pVertices, 0);
	{
		memcpy(pVertices, body, sizeof(tagVertex) * 24);

	}
	vertexBuffer->Unlock();

	DWORD quadIndex[6] = { 0,1,2,0,2,3 };
	DWORD index[36 * 6];
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			index[6 * i + j] = quadIndex[j] + 4 * i;
		}
	}

	DEVICE->CreateIndexBuffer(sizeof(DWORD) * 36 * 6, D3DUSAGE_DYNAMIC, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &indexBuffer, NULL);
	DWORD* pIndices;
	indexBuffer->Lock(0, sizeof(DWORD) * 36 * 6, (void**)&pIndices, 0);
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
