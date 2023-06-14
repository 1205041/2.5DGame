#pragma once
#include "../BaseObject.h"

class Ground :public BaseObject
{
public:
	Ground() { Init(); }
	~Ground() {}

	void Draw()override;
	void Init()override;

private:
	
};