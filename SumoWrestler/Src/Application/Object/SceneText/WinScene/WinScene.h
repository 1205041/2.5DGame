#pragma once

class WinScene :public KdGameObject
{
public:
	WinScene() { Init(); }
	~WinScene() {}

	void DrawUnLit()override;
	void Init()		override;
private:
	KdSquarePolygon m_poly;
	Math::Vector3	m_pos;
};