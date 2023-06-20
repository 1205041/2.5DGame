#pragma once
#include "../SceneBase/SceneBase.h"

class ResultScene :public SceneBase
{
public:
	ResultScene() { Init(); }
	~ResultScene() {}

private:
	void Event()override;
	void Init()	override;

};
