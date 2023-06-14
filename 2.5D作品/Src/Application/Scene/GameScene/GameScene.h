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
	// �J����(��]�����Ȃ��I)
	KdCamera m_camera;

	// �L����
	std::shared_ptr<Player> m_player;
	std::shared_ptr<Enemy>	m_enemy;

	// �n�`
	std::shared_ptr<Ground>		m_ground;
	std::shared_ptr<Scaffold>	m_scaf;
	std::shared_ptr<SkySphere>	m_skySp;
};