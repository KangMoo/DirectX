#include "stdafx.h"
#include "Gizmo.h"

Gizmo* Gizmo::instance = NULL;

Gizmo * Gizmo::Get()
{
	if (instance == NULL)
		instance = new Gizmo;
	return instance;
}

void Gizmo::Destroy()
{
	if (instance != NULL)
		SAFE_DELETE(instance);
}

void Gizmo::DrawLine(const D3DXVECTOR3 & start, const D3DXVECTOR3 & end, D3DXCOLOR color)
{
	D3DXMATRIX prevWorld;

	DEVICE->GetTransform(D3DTS_WORLD, &prevWorld);

	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);

	DEVICE->SetTransform(D3DTS_WORLD, &matWorld);

	GVertex vertex[2];
	vertex[0].position = start;
	vertex[1].position = end;
	vertex[0].color = color;
	vertex[1].color = color;

	DEVICE->SetFVF(GVertex::FVF);
	DEVICE->DrawPrimitiveUP(
		D3DPT_LINELIST,
		1,
		vertex,
		sizeof(GVertex));

	DEVICE->SetTransform(D3DTS_WORLD, &prevWorld);
}

void Gizmo::DrawCircle(const D3DXVECTOR3 & center, float radius, const D3DXVECTOR3 & axis, D3DXCOLOR color)
{
	D3DXMATRIX prevWorld;

	DEVICE->GetTransform(D3DTS_WORLD, &prevWorld);

	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);

	DEVICE->SetTransform(D3DTS_WORLD, &matWorld);

	GVertex vertex[CIRCLEGIZMO_SEGMENTS + 1];
	for (int i = 0; i < CIRCLEGIZMO_SEGMENTS + 1; i++)
	{
		vertex[i].position = circlePos[i];
		vertex[i].color = color;
	}

	//회전 및 크기 변경
	D3DXVECTOR3 forward = axis;
	D3DXVECTOR3 right;
	D3DXVECTOR3 up(0, 1, 0);

	if (forward == up)
		up = D3DXVECTOR3(0, 0, -1);
	else if(forward == D3DXVECTOR3(0,-1,0))
		up = D3DXVECTOR3(0, 0, 1);
	D3DXVec3Cross(&right, &up, &forward);

	DEVICE->SetFVF(GVertex::FVF);
	DEVICE->DrawPrimitiveUP(
		D3DPT_LINELIST,
		36,
		vertex,
		sizeof(GVertex));

	DEVICE->SetTransform(D3DTS_WORLD, &prevWorld);
}

void Gizmo::WorldGrid(int count)
{
	float interval = 1.0f;
	for (int i = -count; i <= count; i++)
	{
		if (i == 0) continue;
		DrawLine
		(
			D3DXVECTOR3(-count * interval, 0, i * interval),
			D3DXVECTOR3(count * interval,0, i *interval),
			0xFF808080
		);
		DrawLine
		(
			D3DXVECTOR3( i * interval,0,  -count * interval),
			D3DXVECTOR3(i * interval, 0, count * interval),
			0xFF808080
		);
	}
	DrawLine
	(
		D3DXVECTOR3(0, 0, -count * interval),
		D3DXVECTOR3(0, 0, count * interval),
		0xFF0000FF
	);
	DrawLine
	(
		D3DXVECTOR3(0,  -count * interval,0),
		D3DXVECTOR3(0, count * interval,0),
		0xFF00FF00
	);
	DrawLine
	(
		D3DXVECTOR3(-count * interval, 0,0),
		D3DXVECTOR3(count * interval, 0,0),
		0xFFFF0000
	);
}

Gizmo::Gizmo()
{
	float intervalAngle = (D3DX_PI * 2.0f) / CIRCLEGIZMO_SEGMENTS;

	for (int i = 0; i <= CIRCLEGIZMO_SEGMENTS; i++)
	{
		float angle = i * intervalAngle;
		circlePos[i] = D3DXVECTOR3(cos(angle), sin(angle), 0);
	}
}


Gizmo::~Gizmo()
{
}
