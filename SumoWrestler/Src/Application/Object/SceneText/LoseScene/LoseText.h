#pragma once
#include "../../ObjBase.h"

class LoseText :public ObjBase
{
public:
	LoseText() { Init(); }
	~LoseText() {}

	void PostUpdate()	override;
	void DrawUnLit()	override;
	void Init()			override;
private:

};