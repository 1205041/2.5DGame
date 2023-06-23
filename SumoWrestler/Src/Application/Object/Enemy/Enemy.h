#pragma once
#include "../ObjBase.h"

class Enemy :public ObjBase
{
public:
	Enemy() { Init(); }
	~Enemy() {}

	void Update()					override;
	void PostUpdate()				override;

	void GenerateDepthMapFromLight()override;
	void DrawLit()					override;

	void Init()						override;
private:
	// 衝突判定とそれに伴う座標の更新
	void UpdateCollision();

	// キャラの移動
	float		m_anime = 0.0f;
	int			m_cnt = 0;
};