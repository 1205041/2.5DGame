#pragma once

class GameScene;

class Player
{
public:
	Player() { Init(); }
	~Player() {}

	void Update();
	void Draw();
	void Init();

	Math::Vector3 GetPos() { return m_pos; }

	void SetGameScene(GameScene* _gameScene) { m_gameScene = _gameScene; }

	void DrawDebug();

private:
	KdSquarePolygon m_poly;
	Math::Matrix	m_mat;
	Math::Vector3	m_pos;
	float			m_anime = 0;

	float			m_gravity = 0.0f;
	bool			m_jumpFlg = false;

	GameScene*		m_gameScene;
	float	maxOverLap = 0.0f;
	bool	hit = false;

	// デバック用
	KdDebugWireFrame m_debugWire;
};