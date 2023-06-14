#pragma once

class Enemy
{
public:
	Enemy() { Init(); }
	~Enemy() {}

	void Update();
	void Draw();
	void Init();

	Math::Vector3 GetPos() { return m_pos; }

	void SetGameScene(GameScene* _gameScene) { m_gameScene = _gameScene; }

	void DrawDebug();

private:
	KdSquarePolygon m_poly;
	Math::Vector3	m_pos;
	Math::Vector3   m_move;
	Math::Matrix	m_mat;
	float			m_anime = 0;

	float		m_gravity = 0.0f;
	int			m_cnt = 0.0f;

	GameScene* m_gameScene;
	float	maxOverLap = 0.0f;
	bool	hit = false;

	// �����蔻��p
	KdCollider m_collider;

	// �f�o�b�N�p
	KdDebugWireFrame m_debugWire;
};