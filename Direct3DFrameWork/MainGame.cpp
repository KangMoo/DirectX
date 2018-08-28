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
	vertex[0].position = D3DXVECTOR3(-1, -1, -1);
	vertex[1].position = D3DXVECTOR3(-1, 1, -1);
	vertex[2].position = D3DXVECTOR3(1, 1, -1);
	vertex[3].position = D3DXVECTOR3(1, -1, -1);

	vertex[4].position = D3DXVECTOR3(-1, -1, 1);
	vertex[5].position = D3DXVECTOR3(-1, 1, 1);
	vertex[6].position = D3DXVECTOR3(1, 1, 1);
	vertex[7].position = D3DXVECTOR3(1, -1, 1);

	vertex[0].color = 0xFFFFFF00;
	vertex[1].color = 0xFF00FF00;
	vertex[2].color = 0xFF0000FF;
	vertex[3].color = 0xFFFFFF00;
	vertex[4].color = 0xFF0000FF;
	vertex[5].color = 0xFF00FF00;
	vertex[6].color = 0xFFFF00FF;
	vertex[7].color = 0xFF0FFFF0;

	//인덱스 설정

	//정면 사각형 인덱스 설정
	DWORD indices[36] = {
		0,1,2,0,2,3,//정면 인덱스 설정
		3,2,6,3,6,7,//측면 인덱스 설정
		7,6,5,7,5,4,//후면 인덱스 설정
		4,5,1,4,1,0,//측면 인덱스 설정
		1,5,6,1,6,2,//측면 인덱스 설정
		4,0,3,4,3,7,//측면 인덱스 설정
	};


	for (int i = 0; i < 36; i++)
		index[i] = indices[i];


	// GPU사용을 위한 전처리 ~

	//vertexBuffer설정 및 초기화 (vertexBuffer에 vertex의 정보값을 넣어준다)
	D3D::Get()->GetDevice()->CreateVertexBuffer(
		sizeof(tagVertex) * 8,		//저장할 데이터의 총 크기 (크기가 클 땐 큰 문제가 없지만, 크기가 작으면 문제가 생김 -> 애매한 정보면 넉넉히 크기 잡아주기)
		D3DUSAGE_DYNAMIC,			//사용처 D3DUSAGE_DYNAMIC : 유동적으로(재초기화 가능하게끔) 데이터를 사용하겠다
		tagVertex::FVF,				//FVF정보
		D3DPOOL_DEFAULT,			//데이터 저장 방식
		&vertexBuffer,				//버퍼 데이터
		NULL						//결과
	);

	//vertexBuffer안에 내용쓰기
	tagVertex* vertices;
	//크기설정
	vertexBuffer->Lock(
		0,						//내용 작성할 시작 지점 (index번호)
		sizeof(tagVertex) * 8,	//사이즈
		(void**)&vertices,		//데이터 받을 변수
		NULL					//플래그
	);
	memcpy(vertices, vertex, sizeof(tagVertex) * 8);	//vertex가 가지고 있는 정보를 vertices에 넘겨줌
	vertexBuffer->Unlock();

	//indexBuffer 초기화
	D3D::Get()->GetDevice()->CreateIndexBuffer(
		sizeof(DWORD) * 36,
		D3DUSAGE_DYNAMIC,
		D3DFMT_INDEX32,
		D3DPOOL_DEFAULT,
		&indexBuffer,
		NULL
	);

	//indexBuffer안에 내용쓰기
	void* pIndices;
	indexBuffer->Lock(0, sizeof(DWORD) * 36, &pIndices, NULL);
	memcpy(pIndices, indices, sizeof(DWORD) * 36);	//indices가 가지고 있는 정보를 pIndices에 넘겨줌
	indexBuffer->Unlock();

	// ~ GPU사용을 위한 전처리

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


	D3DXMatrixIdentity(&matQuatWorld);
	quat[0].position = D3DXVECTOR3(-1, -1, 0);
	quat[1].position = D3DXVECTOR3(-1, 1, 0);
	quat[2].position = D3DXVECTOR3(1, 1, 0);
	quat[3].position = D3DXVECTOR3(1, -1, 0);

	//uv는 0~1범위
	//u:가로 & v:세로
	quat[0].uv = D3DXVECTOR2(0, 1);
	quat[1].uv = D3DXVECTOR2(0, 0);
	quat[2].uv = D3DXVECTOR2(1, 0);
	quat[3].uv = D3DXVECTOR2(1, 1);
	
	//인덱스 초기화
	quatIndex[0] = 0;
	quatIndex[1] = 1;
	quatIndex[2] = 2;

	quatIndex[3] = 0;
	quatIndex[4] = 2;
	quatIndex[5] = 3;
	//D3DXMatrixIdentity(&mat)
	//텍스쳐
	D3DXCreateTextureFromFile(
		D3D::Get()->GetDevice(),	//디바이스
		"temp.png",					//이미지 파일 위치
		&pTex						//저장할 변수
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

	D3DXMATRIX matTrans;
	//D3DXMatrixTranslation : 물체 이동 함수
	D3DXMatrixTranslation(&matTrans, 0, 0, 0);

	D3DXMATRIX matScale;
	//D3DXMatrixScaling : 물체 크기 변경 함수
	D3DXMatrixScaling(&matScale, 1, 1, 1);

	//위치,회전 곱하여 matWorld에 적용
	//SRT 순으로 곱하여 작업한다 (scale, rotation, translation)
	matWorld = matScale * matRotX  * matRotZ *  matRotY * matTrans;
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

	//DrawIndexedPrimitive : vertex를 이용하여 랜더링하는 함수
	//D3D::Get()->GetDevice()->DrawPrimitiveUP(
	//	D3DPT_TRIANGLELIST,	//그릴 도형
	//	2,					//도형의 갯수
	//	vertex,				//그릴 도형의 데이터
	//	sizeof(tagVertex)	//stride 구조체의 크기
	//);

	//0,1,2,3,4,5... 라는 데이터가 입력되었을 때
	//D3DPT_TRINALGELIST : (0,1,2), (3,4,5)... 식으로 그림
	//D3DPT_TRINALGELIP :  (0,1,2), (1,2,3), (2,3,4)... 식으로 그림
	//D3DPT_TRINALGEFAN : (0,1,2), (2,3,4)... 식으로 그림


	//DrawIndexedPrimitiveUP : index & vertex 둘 다 이용하여 랜더링하는 함수
	//D3D::Get()->GetDevice()->DrawIndexedPrimitiveUP(
	//	D3DPT_TRIANGLELIST,	//그릴 도형
	//	0,					//vertex의 시작 인덱스
	//	8,					//vertex 배열의 갯수
	//	12,					//그릴 도형의 갯수
	//	index,				//index 데이터 주소값
	//	D3DFMT_INDEX32,		//index 데이터 1개의 크기값 D3DFMT_INDEX32 : 크기 32짜리 인덱스 (DWORD 는 32)
	//	vertex,				//vertex데이터 주소값
	//	sizeof(tagVertex)	//vertex 구조체 크기
	//);

	// ~~UP : CPU랜더링 & UP이 없는 경우 GPU방식 랜더링
	// DrawPrimitiveUP : CPU방식 랜더링 & DrawPrimitive : GPU방식 랜더링


	
	//GPU를 이용하여 랜더링 ~

	//GPU에 정보 보내기
	//D3D::Get()->GetDevice()->SetStreamSource(
	//	0,					//스트림 번호 0번째 자리
	//	vertexBuffer,		//보낼 데이터
	//	0,					//데이터의 시작지점
	//	sizeof(tagVertex)	//크기
	//);
	//D3D::Get()->GetDevice()->SetIndices(indexBuffer);
	//
	//
	//D3D::Get()->GetDevice()->DrawIndexedPrimitive(
	//	D3DPT_TRIANGLELIST,	//그릴 도형
	//	0,					//vertex의 시작 지점
	//	0,					//vertex의 시작 지점(Buffer에 들어가 있는녀석)
	//	8,					//vertex갯수
	//	0,					//index시작지점
	//	12					//
	//);
	// ~ GPU를 이용하여 랜더링


	D3D::Get()->GetDevice()->SetTransform(D3DTS_WORLD, &matQuatWorld);	//새로운 월드 설정
	
	D3D::Get()->GetDevice()->SetTexture(0, pTex);		//텍스쳐 설정.
	D3D::Get()->GetDevice()->SetFVF(tagVertex2::FVF);
	D3D::Get()->GetDevice()->DrawIndexedPrimitiveUP(
		D3DPT_TRIANGLELIST,	//그릴 도형
		0,					//vertex의 시작 인덱스
		4,					//vertex 배열의 갯수
		2,					//그릴 도형의 갯수
		quatIndex,			//quatIndex 데이터 주소값
		D3DFMT_INDEX32,		//quatIndex 데이터 1개의 크기값
		quat,				//quat데이터 주소값
		sizeof(tagVertex2)	//tagVertex2 구조체 크기
	);

}
