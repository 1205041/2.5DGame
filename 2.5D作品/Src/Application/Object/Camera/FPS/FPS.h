#pragma once
#include "../CameraBase.h"

class FPS :public CameraBase
{
public:
	FPS() { Init(); }
	~FPS() {}

	void Init()		override;
	void Update()	override;

private:
	void UpdateRotateByMouse();
	POINT m_FixMousePos;
};