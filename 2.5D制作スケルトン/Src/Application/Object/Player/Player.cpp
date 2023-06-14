#include "Player.h"

void Player::Update()
{
	// �L�����̃A�j���[�V����
	// 24,25,26��24,25,24,26
	int Run[4] = { 24,25,24,26 };
	m_poly.SetUVRect(Run[(int)m_anime]);
	m_anime += 0.2f;
	if (m_anime >= 4) { m_anime = 0; }

	// �����������Ƃ��� f ��t����
	// �t���ĂȂ�������Adouble�^
	if (GetAsyncKeyState(VK_UP) & 0x8000) { m_pos.z += 0.1f; }
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) { m_pos.x += 0.1f; }
	if (GetAsyncKeyState(VK_DOWN) & 0x8000) { m_pos.z -= 0.1f; }
	if (GetAsyncKeyState(VK_LEFT) & 0x8000) { m_pos.x -= 0.1f; }

	Math::Matrix transMat;
	// �L�����̍��W�s��
	transMat = Math::Matrix::CreateTranslation(m_pos);
	m_mat = transMat;
}

void Player::Draw()
{
	// �|��(�L����)
	KdShaderManager::Instance().m_HD2DShader.DrawPolygon(m_poly, m_mat);
}

void Player::Init()
{
	//	�|��(�L����)������
	m_poly.SetMaterial("Asset/Textures/char.png");
	m_mat = Math::Matrix::Identity;
	m_pos = {};
	m_poly.SetPivot(KdSquarePolygon::PivotType::Center_Bottom);
	m_poly.SetSplit(6, 6);
	m_anime = 0;
}
