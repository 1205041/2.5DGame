#pragma once

class LoseScene :public KdGameObject
{
public:
	LoseScene() { Init(); }
	~LoseScene() {}

	void DrawUnLit()override;
	void Init()		override;
private:
	KdSquarePolygon m_poly;
	Math::Vector3	m_pos;
};