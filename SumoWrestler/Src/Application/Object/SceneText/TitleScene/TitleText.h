#pragma once

class TiteleText :public KdGameObject
{
public:
	TiteleText() { Init(); }
	~TiteleText() {}

	void DrawUnLit()override;
	void Init()		override;
private:
	KdSquarePolygon m_poly;
	Math::Vector3	m_pos;
};