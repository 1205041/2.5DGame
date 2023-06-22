#pragma once

class Enemy :public KdGameObject
{
public:
	Enemy() { Init(); }
	~Enemy() {}

	void Update()					override;
	void PostUpdate()				override;

	void GenerateDepthMapFromLight()override;
	void DrawLit()					override;

	void Init()						override;

	void DrawDebug()				override;

private:
	// 衝突判定とそれに伴う座標の更新
	void UpdateCollision();

	KdSquarePolygon m_poly;
	Math::Vector3   m_moveVec;
	Math::Vector3	m_nowPos;
	float			m_anime = 0.0f;

	float		m_gravity = 0.0f;
	int			m_cnt = 0;

	// 当たり判定用変数
	const float		enableStepHigh	= 0.2f;
	float			maxOverLap		= 0.0f;
	bool			hit				= false;
	Math::Vector3	groundPos;

	// デバック用
	KdDebugWireFrame m_debugWire;
};