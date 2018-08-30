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
	

	MakeCube();

	D3DXMatrixIdentity(&matWorld); //초기화
	D3DXMatrixIdentity(&matView); //초기화
	D3DXMatrixIdentity(&matProjection); //초기화

	//view matrix 만드는 
	D3DXMatrixLookAtLH(
		&matView,	//반환 행렬
		new D3DXVECTOR3(0, 3, -20),
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

	for (int i = 0; i < 3; i++)
		angle[i] = 0.0f;
	D3DXCreateTextureFromFile(
		DEVICE, "download.png", &pTex
	);
	
	pos = D3DXVECTOR3(0, 0, 0);
	
	this->cubeMaterial.Diffuse = D3DXCOLOR(0xFFFF0000);
	this->cubeMaterial.Specular = D3DXCOLOR(0xFFFFFFFF);
	this->cubeMaterial.Ambient = D3DXCOLOR(0xFFFFFFFF);
	this->cubeMaterial.Emissive = D3DXCOLOR(0,1.0,0,1.0f);
	this->cubeMaterial.Power = 10.0f;

	ZeroMemory(&this->light, sizeof(D3DLIGHT9));
	this->light.Type = D3DLIGHT_DIRECTIONAL;
	this->light.Direction = D3DXVECTOR3(-1.0f,0, 0);
	this->light.Diffuse = D3DXCOLOR(1,0,1,1);
	this->light.Specular = D3DXCOLOR(0xFFFFFFFF);

	//D3DXCreateBox();
	//D3DXCreateSphere();
	//D3DXCreateTorus();


	DEVICE->SetRenderState(D3DRS_LIGHTING, TRUE);
	DEVICE->SetLight(0, &this->light);
	DEVICE->LightEnable(0, TRUE);

	DEVICE->SetRenderState(D3DRS_AMBIENT, 0xFF000000);
	DEVICE->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
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
	

	for (int i = 0; i < 7; i++)
		D3DXMatrixIdentity(&matBone[i]);
	for (int i = 0; i < 6;i++)
		D3DXMatrixIdentity(&matCube[i]);

	for (int i = 1; i < 5; i++)
	{
		D3DXMatrixScaling(&matCube[i], 0.1f, 0.1f, 0.1f);
	}
	//몸통 큐브
	D3DXMatrixScaling(&matCube[0], 1.0f, 1.5f, 0.5f);
	D3DXMatrixScaling(&matCube[1], 0.5f, 1.5f, 0.5f);

	D3DXMatrixScaling(&matCube[5], 0.5f, 0.5f, 0.5f);

	matCube[1]._42 -= 0.8f;
	direction = D3DXVECTOR3(0, 0, -1);
	D3DXMATRIX matRootTrans, matRootRot;

	D3DXMatrixRotationY(&matRootRot, angle[0]);
	
	//머리 회전 제한 
	vec[0] = D3DXVECTOR3(1, 0, -1);
	vec[1] = D3DXVECTOR3(-1, 0, -1);
	for (int i = 0; i < 2; i++)
	{
		D3DXVec3Normalize(&vec[i], &vec[i]);
		D3DXVec3TransformNormal(&vec[i], &vec[i], &matRootRot);
	}

	D3DXVec3TransformNormal(&direction, &direction, &matRootRot);

	D3DXMatrixTranslation(&matRootTrans, pos.x, pos.y, pos.z);
	matBone[0] = matRootRot * matRootTrans;
	if (Input->Press('A'))
	{
		angle[0] -= 0.1f;
	}
	if (Input->Press('D'))
	{
		angle[0] += 0.1f;
	}

	if (Input->Press('W'))
	{
		angle[1] += 0.05f;
		pos += direction * 0.5f;
	}
	if (Input->Press('S'))
	{
		angle[1] -= 0.05f;
		pos -= direction * 0.5f;
	}
	//몸통
	matBone[1]._42 = 3;
	//외팔
	matBone[2]._41 = -1.5;
	matBone[2]._42 = 0.8f;
	//오른팔
	matBone[3]._41 = 1;
	matBone[3]._42 = -1;

	//왼다리 
	matBone[4]._41 = -1;
	matBone[4]._42 = -1;
	matBone[5]._41 =  1;
	matBone[5]._42 = -1;
	//머리
	matBone[6]._42 = 2;


	D3DXMATRIX matArm2;
	D3DXMatrixRotationX(&matArm2, angle[1]);
	matBone[2] = matArm2 * matBone[2];

	D3DXVECTOR3 out;
	
	if (!isChange)
	{
		s += 0.05f;
		if (s > 1.0f) isChange = true;
	}
	else
	{
		s -= 0.05f;
		if (s < 0.0f) isChange = false;
	}

	D3DXVec3Lerp(&out, &vec[0], &vec[1], s);

	angle[2] = acosf(out.x / D3DXVec3Length(&out));

	D3DXMATRIX matHeadRot;
	D3DXMatrixRotationY(&matHeadRot, angle[2]);

	matBone[6] = matHeadRot * matBone[6];
	
	//몸통을 root 의 자식으로 
	matBone[1] = matBone[1] * matBone[0];
	//나머지는 몸통의 자식으로 
	for (int i = 2; i < 7;i++)
		matBone[i] *= matBone[1];
	for (int i = 0; i < 6; i++)
		matCube[i] *= matBone[i + 1];
	
}
void MainGame::Render()
{
	GIZMO->WorldGrid(10);

	D3D::Get()->GetDevice()->SetTransform(
		D3DTS_VIEW, &matView);
	D3D::Get()->GetDevice()->SetTransform(
		D3DTS_PROJECTION, &matProjection);

	for (int i = 0; i < 6; i++)
	{
		//큐브의 재질 정보 셋팅 
		//한번 랜더링 명령이 실행될떄 마다 셋팅
		DEVICE->SetMaterial(&cubeMaterial);
		D3D::Get()->GetDevice()->SetTransform(
			D3DTS_WORLD, &matCube[i]);

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
	}
	
}

void MainGame::MakeCube()
{
	tagVertex quad[4];
	
	quad[0].position = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	quad[1].position = D3DXVECTOR3(-1.0f, 1.0f, -1.0f);
	quad[2].position = D3DXVECTOR3(1.0f, 1.0f, -1.0f);
	quad[3].position = D3DXVECTOR3(1.0f, -1.0f, -1.0f);
	
	for (int i = 0; i < 4; i++)
		quad[i].normal = D3DXVECTOR3(0, 0, -1.0f);

	tagVertex cube[24];

	memcpy(cube, quad, sizeof(tagVertex) * 4);

	D3DXMATRIX matRot;
	D3DXMatrixRotationY(&matRot, D3DXToRadian(90.0f));
	for (int i = 0; i < 4; i++)
	{
		D3DXVec3TransformCoord(&quad[i].position, &quad[i].position, &matRot);
		D3DXVec3TransformNormal(&quad[i].normal, &quad[i].normal, &matRot);
	}
	memcpy(cube + 4, quad, sizeof(tagVertex) * 4);

	D3DXMatrixRotationY(&matRot, D3DXToRadian(90.0f));
	for (int i = 0; i < 4; i++)
	{
		D3DXVec3TransformCoord(&quad[i].position, &quad[i].position, &matRot);
		D3DXVec3TransformNormal(&quad[i].normal, &quad[i].normal, &matRot);
	}memcpy(cube + 8, quad, sizeof(tagVertex) * 4);

	D3DXMatrixRotationY(&matRot, D3DXToRadian(90.0f));
	for (int i = 0; i < 4; i++)
	{
		D3DXVec3TransformCoord(&quad[i].position, &quad[i].position, &matRot);
		D3DXVec3TransformNormal(&quad[i].normal, &quad[i].normal, &matRot);
	}memcpy(cube + 12, quad, sizeof(tagVertex) * 4);

	D3DXMatrixRotationZ(&matRot, D3DXToRadian(90.0f));
	for (int i = 0; i < 4; i++)
	{
		D3DXVec3TransformCoord(&quad[i].position, &quad[i].position, &matRot);
		D3DXVec3TransformNormal(&quad[i].normal, &quad[i].normal, &matRot);
	}memcpy(cube + 16, quad, sizeof(tagVertex) * 4);

	D3DXMatrixRotationZ(&matRot, D3DXToRadian(180.0f));
	for (int i = 0; i < 4; i++)
	{
		D3DXVec3TransformCoord(&quad[i].position, &quad[i].position, &matRot);
		D3DXVec3TransformNormal(&quad[i].normal, &quad[i].normal, &matRot);
	}memcpy(cube + 20, quad, sizeof(tagVertex) * 4);


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
