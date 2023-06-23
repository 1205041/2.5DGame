#pragma once
#include "../CameraBase.h"

class TPS :public CameraBase
{
public:
	TPS() { Init(); }
	~TPS() {}

	void Init()		override;
	void Update()	override;

private:
	void UpdateRotateByMouse();
	POINT m_FixMousePos;
};