#pragma once
#include "../ObjBase.h"

class Player;

class Enemy :public ObjBase
{
public:
	Enemy() { Init(); }
	~Enemy() {}

	void Update()					override;
	void PostUpdate()				override;

	void GenerateDepthMapFromLight()override;
	void DrawLit()					override;

	void Init()						override;

	void SetPlayer(const std::shared_ptr<Player>& _player) { m_wpPlayer = _player; }

private:
	// ƒLƒƒƒ‰‚ÌˆÚ“®
	std::weak_ptr<Player> m_wpPlayer;

	const float m_turnDeg = 7.0f;
	float m_deg = 0.0f;
};