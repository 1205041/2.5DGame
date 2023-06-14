#pragma once

class Player;
class Ground;
class Enemy;

class GameScene
{
public:
	GameScene() { Init(); }
	~GameScene(){}

	void Update();
	void PreDraw();
	void Draw();
	void Init();
//	void Release();

private:
	// カメラ(回転させない！)
	KdCamera m_camera;

	// キャラ
	std::shared_ptr<Player> m_player;
	std::shared_ptr<Enemy> m_enemy;

	// 地形
	std::shared_ptr<Ground> m_ground;
};