#include "stdafx.h"
#include "MainGame.h"
#include "DrawImGui.h"
#include "RTT.h"
MainGame::MainGame()
{
	p = new DrawImGui;

}


MainGame::~MainGame()
{
	SAFE_DELETE(p);
}

HRESULT MainGame::Init()
{
	rtt = new RTT(AppDesc.width, AppDesc.height);


	D3DXMatrixLookAtLH(&matView,
		new D3DXVECTOR3(0, 0, -120),
		new D3DXVECTOR3(0, 0, 0),
		new D3DXVECTOR3(0, 1, 0));


	D3DXMatrixPerspectiveFovLH(
		&matProjection, D3DX_PI / 3,
		(float)AppDesc.width / (float)AppDesc.height,
		0.1f, 1000.0f);

	//구 메쉬
	//D3DXCreateSphere(DEVICE, 
	//	1.0f,		//반지름
	//	60.0f,		//세로 디테일
	//	60.0f,      //가로 디테일 
	//	&pMeshSphere, NULL);
	D3DXLoadMeshFromX("sphere.x",
		D3DXMESH_SYSTEMMEM, DEVICE,
		NULL, NULL, NULL, NULL, &pMeshSphere);
	D3DXMatrixTranslation(&matSphere, 0.0f, 0, 0);

	//effect 및 셰이더 초기화
	
	//오류 검사 (왜, 어디)
	LPD3DXBUFFER pError = NULL;

	DWORD dwShaderFlag = 0; //셰이더 옵션

#ifdef _DEBUG
	dwShaderFlag = dwShaderFlag | D3DXSHADER_DEBUG;
#endif // DEBUG

	D3DXCreateEffectFromFile
	(
		DEVICE,		//디바이스 
		"TextureMapping.fx",	//셰이더 코드 파일 이름
		NULL,	//셰이더가 동작하는데 추가할#define
		NULL, //셰이더 추가 #include 
		dwShaderFlag,
		NULL, //매개 변수 공유할 메모리 풀
		&pEffect, // 셰이더 저장
		&pError   // 로딩시 문제있으면 여기로 데이터가 
	);
	if (pError != NULL)
	{
		MessageBox(NULL, (char*)pError->GetBufferPointer(),
			"Shader Error", MB_OK);
		SAFE_RELEASE(pError);
		return E_FAIL;
	}

	D3DXCreateTextureFromFile
	(
		DEVICE,
		"Wall.jpg",
		&pTex
	);


	DEVICE->SetRenderState(D3DRS_LIGHTING, false);
	return S_OK;
}

void MainGame::Release()
{
	SAFE_RELEASE(pEffect);
	SAFE_RELEASE(pMeshSphere);
}

void MainGame::Update()
{
	if (Input->Down(VK_F8))
	{
		D3DXSaveTextureToFile(
			"Test.png",
			D3DXIFF_PNG,
			rtt->GetTexture(),
			NULL
		);
	}
	p->Update();
}
void MainGame::Render()
{
	//effect에 작성된 전역 변수 변수값 셋팅
	this->pEffect->SetMatrix("matWorld", &matSphere);
	this->pEffect->SetMatrix("matView", &matView);
	this->pEffect->SetMatrix("matProjection", &matProjection);
	this->pEffect->SetVector("vMaterialDiffuseColor",
		new D3DXVECTOR4(1, 0, 0, 1));
	this->pEffect->SetVector("vMaterialSpecularColor",
		new D3DXVECTOR4(1, 1, 0, 1));
	this->pEffect->SetTechnique("BaseColor");
	//D3DXMatrixInverse(&matSphere, NULL, &matSphere);
	D3DXVECTOR3 lightDir = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	D3DXVec3Normalize(&lightDir, &lightDir);
	this->pEffect->SetVector("vLightDir",
		new D3DXVECTOR4(lightDir, 1.0f));
	this->pEffect->SetVector("vEyePos",
		new D3DXVECTOR4(0,0, -120, 1.0f));
	this->pEffect->SetFloat("fPower", 60);
	this->pEffect->SetTexture("diffuseTex", pTex);
	DEVICE->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
	//랜더 
	UINT iPassNum = 0;
	this->pEffect->Begin
	(
		&iPassNum,	//pass 수를 받아온다
		0			//옵션
	);
	{
		//pass 수만큼 적용시킨다. 
		for (UINT i = 0; i < iPassNum; i++)
		{
			this->pEffect->BeginPass(i);
			{
				//도형 랜더
				pMeshSphere->DrawSubset(0);

			}
			this->pEffect->EndPass();
		}
	}
	this->pEffect->End();


}

void MainGame::RenderTexutre()
{
	rtt->BeginDraw();
	//effect에 작성된 전역 변수 변수값 셋팅
	this->pEffect->SetMatrix("matWorld", &matSphere);
	this->pEffect->SetMatrix("matView", &matView);
	this->pEffect->SetMatrix("matProjection", &matProjection);
	this->pEffect->SetVector("vMaterialDiffuseColor",
		new D3DXVECTOR4(1, 0, 0, 1));
	this->pEffect->SetVector("vMaterialSpecularColor",
		new D3DXVECTOR4(1, 1, 0, 1));
	this->pEffect->SetTechnique("BaseColor");
	//D3DXMatrixInverse(&matSphere, NULL, &matSphere);
	D3DXVECTOR3 lightDir = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	D3DXVec3Normalize(&lightDir, &lightDir);
	this->pEffect->SetVector("vLightDir",
		new D3DXVECTOR4(lightDir, 1.0f));
	this->pEffect->SetVector("vEyePos",
		new D3DXVECTOR4(0, 0, -120, 1.0f));
	this->pEffect->SetFloat("fPower", 60);
	this->pEffect->SetTexture("diffuseTex", pTex);
	DEVICE->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//랜더 
	UINT iPassNum = 0;
	this->pEffect->Begin
	(
		&iPassNum,	//pass 수를 받아온다
		0			//옵션
	);
	{
		//pass 수만큼 적용시킨다. 
		for (UINT i = 0; i < iPassNum; i++)
		{
			this->pEffect->BeginPass(i);
			{
				//도형 랜더
				pMeshSphere->DrawSubset(0);

			}
			this->pEffect->EndPass();
		}
	}
	this->pEffect->End();

	rtt->EndDraw();
}

void MainGame::GuiUpdate()
{
	p->GuiUpdate();

	ImGui::Image(
		rtt->GetTexture(),
		ImVec2(300,300)
	);
}
