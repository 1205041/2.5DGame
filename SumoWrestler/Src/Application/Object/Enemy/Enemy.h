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
private:
	// �Փ˔���Ƃ���ɔ������W�̍X�V
	void UpdateCollision();

	// �L�����̈ړ�
	float		m_anime = 0.0f;
	int			m_cnt = 0;
};