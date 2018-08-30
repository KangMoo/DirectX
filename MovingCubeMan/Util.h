#pragma once
class Util
{
private:

	static Util* instance;
public:
	static Util * Get();
	static void Destroy();
	void MakeCube(tagVertex* cube, float xsize, float ysize, float zsize, D3DXVECTOR3 center);
private:
	Util();
	~Util();
};

#define UTIL Util::Get()