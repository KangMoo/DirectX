#pragma once

#define CIRCLEGIZMO_SEGMENTS 36
class Gizmo
{
private:
	typedef struct tagGizmoVertex
	{
		D3DXVECTOR3 position;
		DWORD color;
		enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };
	}GVertex, *LPGVertex;

	D3DXVECTOR3 circlePos[CIRCLEGIZMO_SEGMENTS + 1];
	static Gizmo* instance;

public:
	static Gizmo * Get();
	static void Destroy();

	void DrawLine(const D3DXVECTOR3& start, const D3DXVECTOR3& end, D3DXCOLOR color);
	void DrawCircle(const D3DXVECTOR3& center,float radius, const D3DXVECTOR3& axis, D3DXCOLOR color);
	void WorldGrid(int count);
	void WorldGrid(int count,float interval);

private:
	Gizmo();
	~Gizmo();
};

#define GIZMO Gizmo::Get()