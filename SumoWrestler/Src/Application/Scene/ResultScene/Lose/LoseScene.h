#pragma once
#include "../../SceneBase/SceneBase.h"

class LoseScene :public SceneBase
{
public:
	LoseScene() { Init(); }
	~LoseScene() {}

private:
	void Event()override;
	void Init()	override;

};
