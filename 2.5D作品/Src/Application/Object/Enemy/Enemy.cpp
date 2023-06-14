#include "Enemy.h"

void Enemy::Update()
{
	// �f�o�b�N�p
	m_debugWire.AddDebugSphere
	(m_pos + Math::Vector3(0, 0.5f, 0), 0.2f, { 0,1,0,1 });

	// �A�j���[�V����
	int Walk[4] = { 3,4,3,5 };
	m_poly.SetUVRect(Walk[(int)m_anime]);
	m_anime += 0.1f;
	if (m_anime >= 4) { m_anime = 0; }

	Math::Matrix transMat;
	// �L�����̍��W�s��
	transMat = Math::Matrix::CreateTranslation(m_pos);
	m_mat = transMat;
}

void Enemy::Draw()
{
	// �|��(�L����)
	KdShaderManager::Instance().m_HD2DShader.DrawPolygon(m_poly, m_mat);
}

void Enemy::Init()
{
	// �|��(�L����)������
	m_poly.SetMaterial("Asset/Textures/char.png");
	m_mat = Math::Matrix::Identity;
	m_pos = {};
	// �摜���S�_�ړ�
	m_poly.SetPivot(KdSquarePolygon::PivotType::Center_Bottom);
	// �摜����
	m_poly.SetSplit(6, 6);
	m_anime = 0;
}

void Enemy::DrawDebug()
{
}
