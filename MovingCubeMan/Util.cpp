#include"stdafx.h"
#include "Util.h"


Util* Util::instance = NULL;

Util * Util::Get()
{
	if (instance == NULL)
		instance = new Util;
	return instance;
}

void Util::Destroy()
{
	if (instance != NULL)
		SAFE_DELETE(instance);
}

void Util::MakeCube(tagVertex* cube, float xsize, float ysize, float zsize, D3DXVECTOR3 center)
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

	//중심이동
	D3DXMATRIX matTrans;
	D3DXMatrixTranslation(&matTrans, center.x, center.y, center.z);
	for (int i = 0; i < 24; i++)
	{
		D3DXVec3TransformCoord(&cube[i].position, &cube[i].position, &matTrans);
	}
}

Util::Util()
{
}


Util::~Util()
{
}
