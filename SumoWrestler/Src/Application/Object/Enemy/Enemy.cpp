#include "Enemy.h"

void Enemy::Update()
{
	// �����؂��L���ɂ���
	if (m_pos.y < -10) { m_isExpired = true; }

	// �f�o�b�N�p
	m_debugWire.AddDebugSphere
	(
		m_pos + Math::Vector3(0, 0.5f, 0), 
		0.2f, 
		{ 0,1,0,1 }
	);

	// �A�j���[�V����
	int Walk[4] = { 3,4,3,5 };
	m_poly.SetUVRect(Walk[(int)m_anime]);
	m_anime += 0.1f;
	if (m_anime >= 4) { m_anime = 0; }

	m_pos += m_move;
	m_pos.y -= m_gravity;
	m_gravity += 0.005f;

	// ���E�ړ�
	++m_cnt;
	if (m_cnt > 120)
	{
		m_move.x *= -1;
		m_cnt = 0;
	}

	/* ====================== */
	/* �����蔻��(���C����p) */
	/* ====================== */
	KdCollider::RayInfo rayInfo;// ���C�̔��ˈʒu��ݒ�
	rayInfo.m_pos = { 0,-1,0 };	//m_pos�̓L�����̑����̂͂��I
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
	float	maxOverLap = 0.0f;
	bool	hit = false;
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
	sphereInfo.m_type = KdCollider::TypeGround;	 						// �����蔻����������^�C�v��ݒ�

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
	Math::Vector3 hitDir = Math::Vector3::Zero;// �������Ă������
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

void Enemy::PostUpdate()
{
	Math::Matrix transMat;
	// �L�����̍��W�s��
	transMat = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = transMat;
}

void Enemy::GenerateDepthMapFromLight()
{
	// �|��(�e)
	KdShaderManager::Instance().m_HD2DShader.DrawPolygon(m_poly, m_mWorld);
}

void Enemy::DrawLit()
{
	// �|��(�L����)
	KdShaderManager::Instance().m_HD2DShader.DrawPolygon(m_poly, m_mWorld);
}

void Enemy::Init()
{
	// �G�l�~�[������
	m_poly.SetMaterial("Asset/Textures/char.png");
	m_pos = { 3.0f,0,0 };
	m_move = Math::Vector3::Zero;
	m_mWorld = Math::Matrix::Identity;

	// �A�j���[�V����
	m_poly.SetPivot(KdSquarePolygon::PivotType::Center_Bottom);
	m_poly.SetSplit(6, 6);
	m_anime = 0;
}

void Enemy::DrawDebug()
{
	m_debugWire.Draw();
}
