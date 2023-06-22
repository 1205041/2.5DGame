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
	// �Փ˔���Ƃ���ɔ������W�̍X�V
	void UpdateCollision();

	KdSquarePolygon m_poly;
	Math::Vector3   m_moveVec;
	Math::Vector3	m_nowPos;
	float			m_anime = 0.0f;

	float		m_gravity = 0.0f;
	int			m_cnt = 0;

	// �����蔻��p�ϐ�
	const float		enableStepHigh	= 0.2f;
	float			maxOverLap		= 0.0f;
	bool			hit				= false;
	Math::Vector3	groundPos;

	// �f�o�b�N�p
	KdDebugWireFrame m_debugWire;
};