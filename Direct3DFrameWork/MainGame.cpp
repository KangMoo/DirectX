#include "stdafx.h"
#include "MainGame.h"
#include "DrawImGui.h"
#include "RTT.h"

MainGame::MainGame()
{
	pDrawImGui = new DrawImGui;
}


MainGame::~MainGame()
{
	SAFE_DELETE(pDrawImGui);
}

HRESULT MainGame::Init()
{

	rtt = new RTT(AppDesc.width, AppDesc.height);
	D3DXMatrixLookAtLH(&matView,
		new D3DXVECTOR3(2, 2, -5),
		new D3DXVECTOR3(0, 0, 0),
		new D3DXVECTOR3(0, 1, 0));


	D3DXMatrixPerspectiveFovLH(
		&matProjection, D3DX_PI / 3,
		(float)AppDesc.width / (float)AppDesc.height,
		0.1f, 1000.0f);


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

	//모델까지의 경로
	std::string fileName("GunShip/gunship.x");
	std::string filePath = _MESH + fileName;

	LPD3DXBUFFER pAdjacency;		//인접버퍼
	LPD3DXBUFFER pMaterial;			//재질버퍼
	//DWORD dwNumMtrl;				//재질의 수

	D3DXLoadMeshFromX(
		filePath.c_str(),	//경로
		D3DXMESH_MANAGED,	//옵션
		DEVICE,				//디바이스
		&pAdjacency,		//인접정보 버퍼 얻어옴
		&pMaterial,			//인접 재질
		NULL,
		&this->dwMaterialNum,
		&this->pMesh
	);

	//텍스쳐 로딩을 위해서 경로만을 받아온다
	std::string path;
	int lastPathIndex = 0;	//마지막 /의 위치
	lastPathIndex = filePath.find_last_of('/');
	if (lastPathIndex == -1)		// '/'를 찾지 못했을 경우 : -1
	{
		lastPathIndex = filePath.find_last_of('\\');		// 역슬래쉬 찾기
	}

	//경로가 존재하면
	if (lastPathIndex != -1)
	{
		//filepath.substr(시작 위치, 갯수)
		path = filePath.substr(0, lastPathIndex + 1);
	}
	//재질 정보 얻기
	LPD3DXMATERIAL pMaterials = (LPD3DXMATERIAL)pMaterial->GetBufferPointer();
	//재질 정보 갯수 만큼 반복
	for (DWORD i = 0; i < this->dwMaterialNum; i++)
	{
		//로드된 재질
		D3DMATERIAL9 mtrl = pMaterials[i].MatD3D;

		//Diffuse
		//매쉬의 머테리얼에 Ambient 정보가 없다
		//diffuse와 같은 정보 초기화
		mtrl.Ambient = mtrl.Diffuse;
		this->vecMaterials.push_back(mtrl);
		if (pMaterials[i].pTextureFilename != NULL)
		{
			//파일 경로
			std::string texFilePath = path + pMaterials[i].pTextureFilename;
			LPDIRECT3DTEXTURE9 pTex;
			D3DXCreateTextureFromFile(
				DEVICE, texFilePath.c_str(), &pTex
			);
			vecTextures.push_back(pTex);
		}
		else
		{
			//없더라도 반드시 NULL을 대입(중요)
			vecTextures.push_back(NULL);
		}
	}

	//얻어온 재질 정보에 대이터는 다 사용
	//필요없는 녀석은 메모리 해제
	SAFE_RELEASE(pMaterial);

	//매쉬 최적화
	this->pMesh->OptimizeInplace(
		D3DXMESHOPT_ATTRSORT |		//매쉬를 서브셋 순서대로 정렬
		D3DXMESHOPT_COMPACT |		//매쉬에서 사용되지 않는 정점,인덱스 삭제
		D3DXMESHOPT_VERTEXCACHE,	//정점 CASHE 히트율을 높임
		(DWORD*)pAdjacency->GetBufferPointer(),	//인접 버퍼 정보
		NULL,		//최적화를 마치고 결과로 얻는 인접정보
		NULL,		//최적화된 인덱스 정보
		NULL		//최적화된 정점 버퍼 정보
	);
	SAFE_RELEASE(pAdjacency);

	//메쉬 보정 처리 메쉬에서
	//다음과 같은 행렬로 보정시켜
	//모든 데이터를 틀어버린다
	D3DXMATRIX matScale;
	D3DXMatrixScaling(&matScale, 0.2, 0.2, 0.2);
	D3DXMATRIX matRotate;
	D3DXMatrixRotationY(&matRotate, D3DXToRadian(30));
	D3DXMATRIX matCorrection = matScale * matRotate;

	//정점 Element를 얻어 정점 정보 저장
	D3DVERTEXELEMENT9 pVerElement[MAX_FVF_DECL_SIZE];
	pMesh->GetDeclaration(pVerElement);
	int positionOffset = -1;
	int normalOffset = -1;
	int tangentOffset = -1;
	int binormalOffset = -1;
	for (DWORD i = 0; i < MAX_FVF_DECL_SIZE; i++)
	{
		if (pVerElement[i].Type == D3DDECLTYPE_UNUSED)
		{
			//마지막을 만나면 종료
			break;
		}
		if (pVerElement[i].Usage == D3DDECLUSAGE_POSITION)
		{
			positionOffset = pVerElement[i].Offset;
		}
		else if (pVerElement[i].Usage == D3DDECLUSAGE_NORMAL)
		{
			normalOffset = pVerElement[i].Offset;
		}
		else if (pVerElement[i].Usage == D3DDECLUSAGE_TANGENT)
		{
			tangentOffset = pVerElement[i].Offset;
		}
		else if (pVerElement[i].Usage == D3DDECLUSAGE_BINORMAL)
		{
			binormalOffset = pVerElement[i].Offset;
		}
	}
	DWORD verNum = pMesh->GetNumVertices();
	DWORD stride = D3DXGetDeclVertexSize(pVerElement, 0);
	void* p = NULL;
	pMesh->LockVertexBuffer(0, &p);
	{
		for (int i = 0; i < verNum; i++)
		{
			//버텍스 시작 주소
			char* pVertex = ((char*)p + i * stride);
			if (positionOffset != -1)
			{
				D3DXVECTOR3* pos = (D3DXVECTOR3*)(pVertex + positionOffset);
				D3DXVec3TransformCoord(pos, pos, &matCorrection);
			}
			if (normalOffset != -1)
			{
				D3DXVECTOR3* nor = (D3DXVECTOR3*)(pVertex + normalOffset);
				D3DXVec3TransformCoord(nor, nor, &matCorrection);
				D3DXVec3Normalize(nor, nor);
			}
			if (tangentOffset != -1)
			{
				D3DXVECTOR3* nor = (D3DXVECTOR3*)(pVertex + normalOffset);
				D3DXVec3TransformCoord(nor, nor, &matCorrection);
				D3DXVec3Normalize(nor, nor);
			}
			if (binormalOffset != -1)
			{
				D3DXVECTOR3* nor = (D3DXVECTOR3*)(pVertex + normalOffset);
				D3DXVec3TransformCoord(nor, nor, &matCorrection);
				D3DXVec3Normalize(nor, nor);
			}
		}


	}
	pMesh->UnlockAttributeBuffer();

	D3DXMatrixIdentity(&meshLocal);
	D3DXMatrixIdentity(&meshTrans);

	DEVICE->SetRenderState(D3DRS_LIGHTING, false);
	//D3DXMatrixScaling(&meshLocal, 0.5, 0.5, 0.5);
	return S_OK;
}

void MainGame::Release()
{
	SAFE_RELEASE(pEffect);
	for (int i = 0; i < vecTextures.size(); i++)
	{
		SAFE_RELEASE(vecTextures[i]);
	}
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
	pDrawImGui->Update();
}
void MainGame::Render()
{
	DEVICE->SetTransform(D3DTS_WORLD, &meshLocal);
	DEVICE->SetTransform(D3DTS_VIEW, &matView);
	DEVICE->SetTransform(D3DTS_PROJECTION, &matProjection);
	GIZMO->WorldGrid(10);
	//재질 수 만큼 랜더
	for (DWORD i = 0; i < this->dwMaterialNum; i++)
	{
		DEVICE->SetTexture(0, vecTextures[i]);
		this->pMesh->DrawSubset(i);
	}
	DEVICE->SetTexture(0, NULL);

}

void MainGame::RenderTexture()
{

}

void MainGame::GuiUpdate()
{
	pDrawImGui->GuiUpdate();

}
