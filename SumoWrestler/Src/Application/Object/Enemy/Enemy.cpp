#include "Enemy.h"

void Enemy::Update()
{
	// �����؂��L���ɂ���
	if (GetPos().y < -10) { m_isExpired = true; }

	// �f�o�b�N�p
	m_debugWire.AddDebugSphere(GetPos() + Math::Vector3(0, 0.5f, 0), 0.2f, { 0,1,0,1 });

	m_nowPos = GetPos();
	m_nowPos.x += m_moveVec.x;
	m_nowPos.y += m_moveVec.y;
	m_nowPos.y -= m_gravity;
	m_gravity += 0.005f;

	// ���E�ړ�
	++m_cnt;
	if (m_cnt > 120)
	{
		m_moveVec.x *= -1;
		m_cnt = 0;
	}

	// �A�j���[�V����
	int Walk[4] = { 3,4,3,5 };
	m_spPoly->SetUVRect(Walk[(int)m_anime]);
	m_anime += 0.1f;
	if (m_anime >= 4) { m_anime = 0; }
}

void Enemy::PostUpdate()
{
	Math::Matrix transMat;
	// �L�����̍��W�s��
	transMat = Math::Matrix::CreateTranslation(m_nowPos);
	m_mWorld = transMat;
	
	UpdateCollision();
}

void Enemy::GenerateDepthMapFromLight()
{
	// �|��(�e)
	if (!m_spPoly) { return; }

	KdShaderManager::Instance().m_HD2DShader.DrawPolygon(*m_spPoly, m_mWorld);
}

void Enemy::DrawLit()
{
	// �|��(�L����)
	if (!m_spPoly) { return; }

	KdShaderManager::Instance().m_HD2DShader.DrawPolygon(*m_spPoly, m_mWorld);
}

void Enemy::Init()
{
	// �G�l�~�[������
	if (!m_spPoly)
	{
		m_spPoly = std::make_shared<KdSquarePolygon>();
		m_spPoly->SetMaterial(KdAssets::Instance().m_textures.GetData("Asset/Textures/Player/char.png"));

		// �A�j���[�V����
		m_spPoly->SetPivot(KdSquarePolygon::PivotType::Center_Bottom);
		m_spPoly->SetSplit(6, 6);
	}
	m_anime = 0;

	SetPos({ 3.0f,-3.0f,0 });
	m_moveVec = Math::Vector3::Zero;
}

void Enemy::UpdateCollision()
{
	/* ====================== */
	/* �����蔻��(���C����p) */
	/* ====================== */
	KdCollider::RayInfo rayInfo;		// ���C�̔��ˈʒu��ݒ�
	rayInfo.m_pos = GetPos();			//GetPos()�̓L�����̑����̂͂��I
	rayInfo.m_dir = Math::Vector3::Down;// ���C�̔��˕�����ݒ�
	rayInfo.m_pos.y += 0.1f;			// ���������������΂�

	// �i���̋��e�͈�
	rayInfo.m_pos.y += enableStepHigh;
	rayInfo.m_range = m_gravity + enableStepHigh;	// ���C�̒�����ݒ�
	rayInfo.m_type = KdCollider::TypeGround;		// �����蔻����������^�C�v��ݒ�

	/* === �f�o�b�N�p === */
	m_debugWire.AddDebugLine(rayInfo.m_pos, rayInfo.m_dir, rayInfo.m_range);

	// ���C�ɓ��������I�u�W�F�N�g�����i�[���郊�X�g
	std::list<KdCollider::CollisionResult> retRayList;

	/* ���C�Ɠ����蔻������� */
	for (auto& obj : SceneManager::Instance().GetObjList()) { obj->Intersects(rayInfo, &retRayList); }

	// ���C�ɓ����������X�g�����ԋ߂��I�u�W�F�N�g�����o
	maxOverLap	= 0.0f;
	hit			= false;
	groundPos	= Math::Vector3::Zero;
	for (auto& ret : retRayList)
	{
		// ���C���Ւf���ăI�[�o�[������������Ԓ������̂�T��
		if (maxOverLap < ret.m_overlapDistance)
		{
			maxOverLap	= ret.m_overlapDistance;
			groundPos	= ret.m_hitPos;
			hit			= true;
		}
	}
	if (hit)
	{
		// �n�ʂɓ������Ă���
		Math::Vector3	hitPos =
			groundPos + Math::Vector3(0, -0.1f, 0);
		SetPos(hitPos);
		m_gravity = 0;
	}
	else
	{
		notHitCnt++;
		if (notHitCnt >= 60)
		{
			SceneManager::Instance().SetNextScene(SceneManager::SceneType::Win);
		}
	}

	/* ==================== */
	/* �����蔻��(������p) */
	/* ==================== */
	KdCollider::SphereInfo sphereInfo;		// ������p�̕ϐ�
	sphereInfo.m_sphere.Center = GetPos() + Math::Vector3(0, 0.5f, 0);	// ���̒��S�ʒu��ݒ�
	sphereInfo.m_sphere.Radius = 0.3f;									// ���̔��a��ݒ�
	sphereInfo.m_type = KdCollider::TypeDamage;		// �����蔻����������^�C�v��ݒ�

	/* === �f�o�b�N�p(��) === */
	m_debugWire.AddDebugSphere(sphereInfo.m_sphere.Center, sphereInfo.m_sphere.Radius);

	// ���ɓ��������I�u�W�F�N�g�����i�[���郊�X�g
	std::list<KdCollider::CollisionResult> retSphereList;

	// ���Ɠ����蔻��
	for (auto& obj : SceneManager::Instance().GetObjList()) { obj->Intersects(sphereInfo, &retSphereList); }

	// ���ɓ����������X�g�����ԋ߂��I�u�W�F�N�g�����o
	maxOverLap	= 0.0f;
	hit			= false;
	Math::Vector3 hitDir = Math::Vector3::Zero;// �������Ă������
	for (auto& ret : retSphereList)
	{
		// ��Ԃ߂荞�񂾃I�u�W�F�N�g��T��
		if (maxOverLap < ret.m_overlapDistance)
		{
			maxOverLap	= ret.m_overlapDistance;
			hitDir		= ret.m_hitDir;// �����Ԃ�����
			hit			= true;
		}
	}
	if (hit)
	{
		// �����Ԃ�
		Math::Vector3 newPos =
			GetPos() + (hitDir * maxOverLap);
		SetPos(newPos);
	}
}
