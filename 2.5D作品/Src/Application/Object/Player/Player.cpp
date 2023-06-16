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
	if(GetAsyncKeyState(VK_DOWN) & 0x8000) { m_pos.z -= 0.1f; }
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
	KdCollider::RayInfo rayInfo;// ���C�̔��ˈʒu��ݒ�
	rayInfo.m_pos = m_pos;		//m_pos�̓L�����̑����̂͂��I
	rayInfo.m_dir = { 0,-1,0 };	// ���C�̔��˕�����ݒ�
	rayInfo.m_pos.y += 0.1f;	// ���������������΂�
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
	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		obj->Intersects
		(
			rayInfo,	// ���C�̏��
			&retRayList	// �������������i�[���郊�X�g
		);
	}

	// ���C�ɓ����������X�g�����ԋ߂��I�u�W�F�N�g�����o
	float	maxOverLap  = 0.0f;
	bool	hit			= false;
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

	/* ==================== */
	/* �����蔻��(������p) */
	/* ==================== */
	KdCollider::SphereInfo sphereInfo;		// ������p�̕ϐ�
	sphereInfo.m_sphere.Center = m_pos + Math::Vector3(0, 0.5f, 0);		// ���̒��S�ʒu��ݒ�
	sphereInfo.m_sphere.Radius = 0.3f;									// ���̔��a��ݒ�
	sphereInfo.m_type = KdCollider::TypeGround | KdCollider::TypeBump;	// �����蔻����������^�C�v��ݒ�

	/* === �f�o�b�N�p(��) === */
	m_debugWire.AddDebugSphere(sphereInfo.m_sphere.Center, sphereInfo.m_sphere.Radius);

	// ���ɓ��������I�u�W�F�N�g�����i�[���郊�X�g
	std::list<KdCollider::CollisionResult> retSphereList;

	// ���Ɠ����蔻��

	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		obj->Intersects
		(
			sphereInfo,		// ���̏��
			&retSphereList	// �������������i�[���郊�X�g
		);
	}

	// ���ɓ����������X�g�����ԋ߂��I�u�W�F�N�g�����o
	maxOverLap = 0.0f;
	hit = false;
	Math::Vector3 hitDir = {};// �������Ă������
	for (auto& ret : retSphereList)
	{
		// ��Ԃ߂荞�񂾃I�u�W�F�N�g��T��
		if (maxOverLap < ret.m_overlapDistance)
		{
			maxOverLap = ret.m_overlapDistance;
			hit = true;
			hitDir = ret.m_hitDir;// �����Ԃ�����
		}
	}
	if (hit)
	{
		// Z�ւ̉����Ԃ��}�~
		hitDir.z = 0;
		hitDir.Normalize();

		// �����Ԃ�
		m_pos += hitDir * maxOverLap;
	}
}

void Player::PostUpdate()
{
	Math::Matrix transMat;
	// �L�����̍��W�s��
	transMat = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = transMat;
}

void Player::GenerateDepthMapFromLight()
{
	// �|��(�e)
	KdShaderManager::Instance().m_HD2DShader.DrawPolygon(m_poly, m_mWorld);
}

void Player::DrawLit()
{
	// �|��(�L����)
	KdShaderManager::Instance().m_HD2DShader.DrawPolygon(m_poly, m_mWorld);
}

void Player::Init()
{
	// �L���������l
	m_poly.SetMaterial("Asset/Textures/char.png");
	m_mWorld = Math::Matrix::Identity;
	m_pos = { -20,0,0 };

	// �A�j���[�V����������
	m_poly.SetPivot(KdSquarePolygon::PivotType::Center_Bottom);
	m_poly.SetSplit(6, 6);
	m_anime = 0.0f;
}

void Player::DrawDebug()
{
	m_debugWire.Draw();
}
