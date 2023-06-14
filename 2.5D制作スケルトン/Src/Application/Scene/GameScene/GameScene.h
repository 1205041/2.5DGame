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
	// �J����(��]�����Ȃ��I)
	KdCamera m_camera;

	// �L����
	std::shared_ptr<Player> m_player;
	std::shared_ptr<Enemy> m_enemy;

	// �n�`
	std::shared_ptr<Ground> m_ground;
};