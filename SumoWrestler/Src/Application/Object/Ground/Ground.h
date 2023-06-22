#pragma once

class Ground :public KdGameObject
{
public:
	Ground() { Init(); }
	~Ground() {}

	void Update()		override;
	void PostUpdate()	override;
	void DrawLit()		override;
	void Init()			override;
	void DrawDebug()	override;
private:
	std::shared_ptr<KdModelWork> m_model;

	float	maxOverLap	= 0.0f;
	bool	hit			= false;

	// デバック用
	KdDebugWireFrame m_debugWire;
};