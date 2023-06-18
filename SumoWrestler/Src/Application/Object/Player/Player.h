#pragma once

class Player :public KdGameObject
{
public:
	Player() { Init(); }
	~Player() {}

	void Update()					override;
	void PostUpdate()				override;

	void GenerateDepthMapFromLight()override;
	void DrawLit()					override;

	void Init()						override;

	void DrawDebug()				override;

private:
	KdSquarePolygon m_poly;
	Math::Vector3	m_pos;
	Math::Vector3	m_moveDir;
	float			m_speed = 0.1f;
	float			m_anime = 0.0f;

	float			m_gravity = 0.0f;
	bool			m_jumpFlg = false;

	// デバック用
	KdDebugWireFrame m_debugWire;
};