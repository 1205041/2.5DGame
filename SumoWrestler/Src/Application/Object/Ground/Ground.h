#pragma once

class Ground :public KdGameObject
{
public:
	Ground() { Init(); }
	~Ground() {}

	void PostUpdate()	override;
	void DrawLit()		override;
	void Init()			override;

private:
	std::shared_ptr<KdModelWork> m_model;

	// デバック用
	KdDebugWireFrame m_debugWire;
};