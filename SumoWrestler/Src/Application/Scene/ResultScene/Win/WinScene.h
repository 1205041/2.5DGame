#pragma once
#include "../../SceneBase/SceneBase.h"

class WinScene :public SceneBase
{
public:
	WinScene() { Init(); }
	~WinScene() {}

private:
	void Event()override;
	void Init()	override;

};