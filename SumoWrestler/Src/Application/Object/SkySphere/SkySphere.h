#pragma once
#include "../ObjBase.h"

class SkySphere :public ObjBase
{
public:
	SkySphere() { Init(); }
	~SkySphere() {};

	void Update()		override;
	void PostUpdate()	override;
	void DrawUnLit()	override;
	void Init()			override;

private:
	float m_cnt = 0.0f;
};
