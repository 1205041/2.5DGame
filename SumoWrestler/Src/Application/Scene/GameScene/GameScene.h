#pragma once
#include "../SceneBase/SceneBase.h"

class Player;

class GameScene :public SceneBase
{
public:
	GameScene() { Init(); }
	~GameScene(){}

private:
	void Event()override;
	void Init()	override;

	// カメラのためのプレイヤー情報
	std::weak_ptr<Player> m_wpPlayer;
};