#include "Player.h"

#include "../../Scene/GameScene/GameScene.h"
#include "../Ground/Ground.h"

void Player::Update()
{
	// �L�����̃A�j���[�V����
	// 24,25,26��24,25,24,26
	int Run[4] = { 24,25,24,26 };
	m_poly.SetUVRect(Run[(int)m_anime]);
	m_anime += 0.2f;
	if (m_anime >= 4) { m_anime = 0; }

	if (GetAsyncKeyState(VK_UP) & 0x8000) { m_pos.z += 0.1f; }
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) { m_pos.x += 0.1f; }
	if (GetAsyncKeyState(VK_DOWN) & 0x8000) { m_pos.z -= 0.1f; }
	if (GetAsyncKeyState(VK_LEFT) & 0x8000) { m_pos.x -= 0.1f; }
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		if (!m_jumpFlg)
		{
			m_gravity = -0.1f;
			m_jumpFlg = true;
		}
	}
	else
	{
		m_jumpFlg = false;
	}
	m_pos.y -= m_gravity;
	m_gravity += 0.005f;

	/* ====================== */
	/* �����蔻��(���C����p) */
	/* ====================== */
	KdCollider::RayInfo rayInfo;
	// ���C�̔��ˈʒu��ݒ�
	rayInfo.m_pos = m_pos;//m_pos�̓L�����̑����̂͂��I
	// ���C�̔��˕�����ݒ�
	rayInfo.m_dir = { 0,-1,0 };
	// ���������������΂�
	rayInfo.m_pos.y += 0.1f;
	// �i���̋��e�͈�
	static float enableStepHigh = 0.2f;
	rayInfo.m_pos.y += enableStepHigh;

	// ���C�̒�����ݒ�
	rayInfo.m_range = m_gravity + enableStepHigh;
	// �����蔻����������^�C�v��ݒ�
	rayInfo.m_type = KdCollider::TypeGround;

	/* === �f�o�b�N�p === */
	m_debugWire.AddDebugLine
	(
		rayInfo.m_pos,
		rayInfo.m_dir,
		rayInfo.m_range
	);

	// ���C�ɓ��������I�u�W�F�N�g�����i�[���郊�X�g
	std::list<KdCollider::CollisionResult> retRayList;

	/* ���C�Ɠ����蔻������� */
	// Ground�̃��C���A�Ώۂ̍s��A�i�[���郊�X�g
	m_gameScene->GetGround()->GetCollidr().Intersects
	(
		rayInfo,		// ���C�̏��,
		m_gameScene->GetGround()->GetMat(), // �Ώۂ̍s��,
		&retRayList		// �������������i�[���郊�X�g
	);

	// Scaffold�̃��C���A�Ώۂ̍s��A�i�[���郊�X�g
//	m_gameScene->GetScaffold()->GetCollidr().Intersects
//	(
//		rayInfo,
//		m_gameScene->GetScaffold()->GetMat(),
//		&retRayList
//	);

	// ���C�ɓ����������X�g�����ԋ߂��I�u�W�F�N�g�����o
	maxOverLap = 0.0f;
	hit = false;
	Math::Vector3 groundPos = Math::Vector3::Zero;
	for (auto& ret : retRayList)
	{
		// ���C���Ւf���ăI�[�o�[����������
		// ��Ԓ������̂�T��
		if (maxOverLap < ret.m_overlapDistance)
		{
			maxOverLap = ret.m_overlapDistance;
			groundPos = ret.m_hitPos;
			hit = true;
		}
	}
	if (hit)
	{
		// �n�ʂɓ������Ă���
		m_pos = groundPos + Math::Vector3(0, -0.1f, 0);
		m_gravity = 0;
	}

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
	m_anime = 0.0f;
}

void Player::DrawDebug()
{
	m_debugWire.Draw();
}
