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

	void SetPos(Math::Vector3 _pos) { m_pos = _pos; }
	
private:
	// �Փ˔���Ƃ���ɔ������W�̍X�V
	void UpdateCollision();

	KdSquarePolygon m_poly;
	Math::Vector3	m_pos;
	Math::Vector3   m_move;
	float			m_anime = 0.0f;

	float		m_gravity = 0.0f;
	int			m_cnt = 0;

	// �f�o�b�N�p
	KdDebugWireFrame m_debugWire;
};