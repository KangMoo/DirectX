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
	vertex[0].position = D3DXVECTOR3(-1, 0, 0);
	vertex[1].position = D3DXVECTOR3(0, 1, 0);
	vertex[2].position = D3DXVECTOR3(1, 0, 0);

	vertex[0].color = 0xFFFF0000;
	vertex[1].color = 0xFF00FF00;
	vertex[2].color = 0xFF0000FF;

	D3D::Get()->GetDevice()->SetRenderState(
		D3DRS_LIGHTING, false
	);

	return S_OK;
}

void MainGame::Release()
{
}

void MainGame::Update()
{
}

void MainGame::Render()
{
	D3D::Get()->GetDevice()->SetFVF(tagVertex::FVF);
	D3D::Get()->GetDevice()->DrawPrimitiveUP(
		D3DPT_TRIANGLELIST,
		1,
		vertex,
		sizeof(tagVertex)
	);
}
