#pragma once

class LoseText :public KdGameObject
{
public:
	LoseText() { Init(); }
	~LoseText() {}

	void DrawUnLit()override;
	void Init()		override;
private:
	KdSquarePolygon m_poly;
	Math::Vector3	m_pos;
};