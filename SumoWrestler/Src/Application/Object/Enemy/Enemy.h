#pragma once
#include "../ObjBase.h"

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

	void SetPlayerPos(Math::Vector3 _playerPos) { playerPos = _playerPos; }

private:
	// �Փ˔���Ƃ���ɔ������W�̍X�V
	void UpdateCollision();

	// �L�����̈ړ�
	Math::Vector3	playerPos;
	float			m_ang = 0.0f;

	float		m_anime = 0.0f;
};