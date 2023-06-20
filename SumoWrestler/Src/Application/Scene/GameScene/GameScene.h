#pragma once
#include "../SceneBase/SceneBase.h"

class GameScene :public SceneBase
{
public:
	GameScene() { Init(); }
	~GameScene(){}

private:
	void Event()override;
	void Init()	override;
};