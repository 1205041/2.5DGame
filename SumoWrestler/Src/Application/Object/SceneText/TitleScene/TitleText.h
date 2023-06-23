#pragma once
#include "../../ObjBase.h"

class TitleText :public ObjBase
{
public:
	TitleText() { Init(); }
	~TitleText() {}

	void PostUpdate()	override;
	void DrawUnLit()	override;
	void Init()			override;
private:

};