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
	for (int i = 0; i < 4; i++)
	{
		D3DXMatrixIdentity(&childWorld[i]);
	}
	//view matrix 만드는 
	D3DXMatrixLookAtLH(
		&matView,	//반환 행렬
		new D3DXVECTOR3(2, 5, -10),
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
	
	//D3DXCreateTextureFromFile(
	//	DEVICE, "download.png", &pTex
	//);
	xAngle = 0;
	yAngle = 0;
	zAngle = 0;
	pos = D3DXVECTOR3(0, 0, 0);
	this->material.Diffuse = D3DXCOLOR(0xFFAACCEE);
	this->material.Ambient = D3DXCOLOR(0xFFAACCEE);
	this->material.Specular = D3DXCOLOR(0.2, 0.2, 0.2, 0.1f);
	this->material.Emissive = D3DXCOLOR(0.2, 0.2, 0.2, 0.1f);
	this->material.Power = 0.00005;

	ZeroMemory(&this->light, sizeof(D3DLIGHT9));
	this->light.Type = D3DLIGHT_DIRECTIONAL;
	this->light.Diffuse = D3DXCOLOR(1, 1, 1, 1);
	this->light.Specular = D3DXCOLOR(1,1,1, 0.000000000001);

	this->light.Direction = lightdirection = D3DXVECTOR3(1,-1,1);


	DEVICE->SetLight(0, &this->light);
	DEVICE->LightEnable(0, TRUE);
	DEVICE->SetRenderState(D3DRS_LIGHTING, TRUE);

	DEVICE->SetRenderState(D3DRS_AMBIENT, 0xFF000000);
	DEVICE->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
	return S_OK;
}

void MainGame::Update()
{

	D3DXCreateBox(DEVICE, 1, 1, 1, &mesh[0], NULL);
	D3DXCreateSphere(DEVICE, 0.75, 50, 30, &mesh[1], NULL);
	D3DXCreateTeapot(DEVICE, &mesh[2], NULL);
	D3DXCreateTorus(DEVICE, 0.2, 0.5, 100, 30, &mesh[3], NULL);
	if (Input->Press(VK_UP))
	{

		if (Input->Press('X'))
			lightdirection.x += 0.1;
		if (Input->Press('Y'))
			lightdirection.y += 0.1;
		if (Input->Press('Z'))
			lightdirection.z += 0.1;
	}
	else if (Input->Press(VK_DOWN))
	{

		if (Input->Press('X'))
			lightdirection.x -= 0.1;
		if (Input->Press('Y'))
			lightdirection.y -= 0.1;
		if (Input->Press('Z'))
			lightdirection.z -= 0.1;
	}
	else
	{
		if (Input->Press('X'))
			xAngle += 0.1;
		if (Input->Press('Y'))
			yAngle += 0.1;
		if (Input->Press('Z'))
			zAngle += 0.1;
	}

	if (Input->Down('R'))
	{
		xAngle = yAngle = zAngle = 0;
		lightdirection = D3DXVECTOR3(1, -1, 1);
	}
	this->light.Direction = lightdirection;

	DEVICE->SetLight(0, &this->light);
	D3DXMATRIX matRotX, matRotY, matRotZ;
	D3DXMatrixRotationX(&matRotX, xAngle);
	D3DXMatrixRotationY(&matRotY, yAngle);
	D3DXMatrixRotationZ(&matRotZ, zAngle);
	matWorld = matRotX * matRotY * matRotZ;
	D3DXMATRIX matTrans[4];
	D3DXMatrixTranslation(&matTrans[0], -2, -2, 0);
	D3DXMatrixTranslation(&matTrans[1],  2, -2, 0);
	D3DXMatrixTranslation(&matTrans[2], -2,  2, 0);
	D3DXMatrixTranslation(&matTrans[3],  2,  2, 0);
	for (int i = 0; i < 4; i++)
	{
		childWorld[i] = matRotX * matRotY * matRotZ * matTrans[i];
	}

}
void MainGame::Render()
{
	//GIZMO->WorldGrid(10, 10);
	DEVICE->SetMaterial(&material);
	D3D::Get()->GetDevice()->SetTransform(
		D3DTS_WORLD, &matWorld);
	D3D::Get()->GetDevice()->SetTransform(
		D3DTS_VIEW, &matView);
	D3D::Get()->GetDevice()->SetTransform(
		D3DTS_PROJECTION, &matProjection);

	D3D::Get()->GetDevice()->SetFVF(tagVertex::FVF);


	DEVICE->BeginScene();

	DEVICE->SetTransform(D3DTS_WORLD, &childWorld[0]);
	mesh[0]->DrawSubset(0);

	DEVICE->SetTransform(D3DTS_WORLD, &childWorld[1]);
	mesh[1]->DrawSubset(0);
	DEVICE->SetTransform(D3DTS_WORLD, &childWorld[2]);
	mesh[2]->DrawSubset(0);
	DEVICE->SetTransform(D3DTS_WORLD, &childWorld[3]);
	mesh[3]->DrawSubset(0);
	DEVICE->EndScene();
	
	char str[128];
	sprintf(str, "현재 광원 방향 : X:%f, Y:%f, Z:%f", lightdirection.x, lightdirection.y, lightdirection.z);


	FONT->PrintText(str, 20, 40, 0xFFFFFFFF);
	
	//DEVICE->SetTexture(0, pTex);
	//D3D::Get()->GetDevice()->SetStreamSource(
	//	0,		//스트림 번호 0번째 자리 
	//	vertexBuffer,
	//	0,		//바이트 시작지점
	//	sizeof(tagVertex)
	//);
	//
	//D3D::Get()->GetDevice()->SetIndices(indexBuffer);
	//
	//D3D::Get()->GetDevice()->DrawIndexedPrimitive(
	//	D3DPT_TRIANGLELIST //그릴도형
	//	, 0				  // vertex시작
	//	, 0				  //vertex시작지점
	//	, 24			//
	//	, 0				//
	//	, 12			//
	//);
	//D3D::Get()->GetDevice()->SetTransform(
	//	D3DTS_WORLD, &childWorld[0]);
	//D3D::Get()->GetDevice()->DrawIndexedPrimitive(
	//	D3DPT_TRIANGLELIST //그릴도형
	//	, 0				  // vertex시작
	//	, 0				  //vertex시작지점
	//	, 24			//
	//	, 0				//
	//	, 12			//
	//);
	//D3D::Get()->GetDevice()->SetTransform(
	//	D3DTS_WORLD, &childWorld[1]);
	//D3D::Get()->GetDevice()->DrawIndexedPrimitive(
	//	D3DPT_TRIANGLELIST //그릴도형
	//	, 0				  // vertex시작
	//	, 0				  //vertex시작지점
	//	, 24			//
	//	, 0				//
	//	, 12			//
	//);
	//D3D::Get()->GetDevice()->SetTransform(
	//	D3DTS_WORLD, &childWorld[2]);
	//D3D::Get()->GetDevice()->DrawIndexedPrimitive(
	//	D3DPT_TRIANGLELIST //그릴도형
	//	, 0				  // vertex시작
	//	, 0				  //vertex시작지점
	//	, 24			//
	//	, 0				//
	//	, 12			//
	//);
	//D3D::Get()->GetDevice()->SetTransform(
	//	D3DTS_WORLD, &childWorld[3]);
	//D3D::Get()->GetDevice()->DrawIndexedPrimitive(
	//	D3DPT_TRIANGLELIST //그릴도형
	//	, 0				  // vertex시작
	//	, 0				  //vertex시작지점
	//	, 24			//
	//	, 0				//
	//	, 12			//
	//);
	//FONT->PrintText("Font Test", 20, 40, 0xFFFFFFFF);
	//GIZMO->DrawLine(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 1, 0), 0XFF00FFFF);
}

void MainGame::MakeCube()
{

	tagVertex body[24];
	UTIL->MakeCube(body, 1, 1, 3, D3DXVECTOR3(0, 0.5, -1.5));

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

void MainGame::Release()
{
	vertexBuffer->Release();
	vertexBuffer = 0;
	indexBuffer->Release();
	indexBuffer = 0;
	pTex->Release();
	pTex = 0;
}
