#pragma once
#include "../BaseObject.h"

class Enemy :public BaseObject
{
public:
	Enemy() { Init(); }
	~Enemy() {}

	void Update()override;
	void Draw()override;
	void Init()override;

	Math::Vector3 GetPos() { return m_pos; }

private:
	
};