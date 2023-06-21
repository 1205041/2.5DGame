#pragma once
#include "../CameraBase/CameraBase.h"

class Tracking : public CameraBase
{
public:
	Tracking() { Init(); }
	~Tracking() {}

	void Init()					override;
	void Update()				override;
};