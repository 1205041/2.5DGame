#pragma once
#include "../BaseObject.h"

class Player :public BaseObject
{
public:
	Player() { Init(); }
	~Player() {}

	void Update()override;
	void Draw()override;
	void Init()override;

	Math::Vector3 GetPos() { return m_pos; }
	// ������̍��W�̎���
//	m_pos;
//	m_mat.Translation();
 	// �g�����̈��(�Z�b�^�[,�Q�b�^�[���p�Ƃ��ė��p)
//	Math::Vector3 pos = m_mat.Translation();
//	m_mat.Translation({ 0,1,0 });

private:
	
};