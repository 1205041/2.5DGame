#pragma once
#include "../../ObjBase.h"

class WinText :public ObjBase
{
public:
	WinText() { Init(); }
	~WinText() {}

	void PostUpdate()override;
	void DrawUnLit()override;
	void Init()		 override;
private:

};