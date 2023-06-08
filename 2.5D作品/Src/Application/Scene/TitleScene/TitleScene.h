#pragma once
#include "../SceneBase/SceneBase.h"

class TitleScene :public SceneBase
{
public:
	TitleScene() { Init(); }
	~TitleScene() {}

private:
	void Init()	override;
	void Event()override;
};