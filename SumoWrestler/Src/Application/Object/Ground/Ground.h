#pragma once
#include "../ObjBase.h"

class Ground :public ObjBase
{
public:
	Ground() { Init(); }
	~Ground() {}

	void PostUpdate()	override;
	void DrawLit()		override;
	void Init()			override;

private:

};