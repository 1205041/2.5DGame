#pragma once

class SkySphere :public KdGameObject
{
public:
	SkySphere() { Init(); }
	~SkySphere() {};

	void PostUpdate()override;
	void DrawUnLit() override;
	void Init()		override;

private:
	std::shared_ptr<KdModelWork> m_model;
	
};
