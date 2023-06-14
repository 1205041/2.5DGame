#pragma once
#include "../SceneBase/SceneBase.h"

class Player;
class Enemy;
class Ground;
class Scaffold;
class SkySphere;

class GameScene :public SceneBase
{
public:
	GameScene() { Init(); }
	~GameScene(){}

	void Update();
	void PreDraw();
	void Draw();
	void Init();

	const std::shared_ptr<Ground>& GetGround() { return m_ground; }
//	const std::shared_ptr<Scaffold>& GetScaffold() { return m_scaf; }
	const std::shared_ptr<Enemy>& GetEnemy() { return m_enemy; }

private:
	// カメラ(回転させない！)
	KdCamera m_camera;

	// キャラ
	std::shared_ptr<Player> m_player;
	std::shared_ptr<Enemy>	m_enemy;

	// 地形
	std::shared_ptr<Ground>		m_ground;
	std::shared_ptr<Scaffold>	m_scaf;
	std::shared_ptr<SkySphere>	m_skySp;
};