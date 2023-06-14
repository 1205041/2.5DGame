#pragma once

class BaseObject
{
public:
	BaseObject(){}
	~BaseObject() { Release(); }

	virtual void Update();
	virtual void Draw();
	virtual void Init();
private:
	void Release();

protected:
	KdSquarePolygon m_poly;
	Math::Matrix	m_mat;
	Math::Vector3	m_pos;
	float			m_anime = 0;
};