#pragma once

class SkySphere :public KdGameObject
{
public:
	SkySphere() { Init(); }
	~SkySphere() {};

	void PostUpdate()				 override;
	
	void GenerateDepthMapFromLight() override;
	void DrawLit()					 override;
	
	void Init()						 override;

private:
	std::shared_ptr<KdModelWork> m_model;
	
};
