#pragma once
#include "../SceneBase/SceneBase.h"

class ResultScene :public SceneBase
{
public:
	ResultScene() { Init(); }
	~ResultScene() {}

private:
	void Init()	override;
	void Event()override;

};