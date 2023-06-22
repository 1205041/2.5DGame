#pragma once

class WinText :public KdGameObject
{
public:
	WinText() { Init(); }
	~WinText() {}

	void DrawSprite()override;
	void Init()		 override;
private:
	KdSquarePolygon m_poly;
	Math::Vector3	m_pos;
};