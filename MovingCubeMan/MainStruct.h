#pragma once
#include "stdafx.h"
#define PI 3.1415926535
struct tagVertex {
	D3DXVECTOR3 position; //무조건 작성이 되어있어야 된다.
	D3DXVECTOR2 uv;
	enum { FVF = D3DFVF_XYZ | D3DFVF_TEX1 };
};