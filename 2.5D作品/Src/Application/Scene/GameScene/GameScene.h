#pragma once
#include "../SceneBase/SceneBase.h"

class GameScene :public SceneBase
{
public:
	GameScene() { Init(); }
	~GameScene() {}

private:
	void Init() override;
	void Event()override;
};