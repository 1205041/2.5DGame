#include "Enemy.h"
#include "../Player/Player.h"

void Enemy::Update()
{
	// �����؂��L���ɂ���
	if (GetPos().y < -10) { m_isExpired = true; }
	if(m_isExpired)
	{
		KdAudioManager::Instance().StopAllSound();
		SceneManager::Instance().SetNextScene(SceneManager::SceneType::Win);
	}

	m_gravity += 0.01f;
	m_mWorld._42 -= m_gravity;

	// �Ǐ]�ړ�
	m_nowPos = GetPos();

	m_moveVec = Math::Vector3::Zero;
	
	m_moveVec.Normalize();
	m_moveVec *= m_moveSpd;

	m_nowPos += m_moveVec;

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

	m_moveSpd = 0.05f;
	m_moveVec = Math::Vector3::Zero;

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("EnemyCollider", GetPos(), 0.25f, KdCollider::TypeBump);
}

void Enemy::UpdateCollision()
{
	/* ====================== */
	/* �����蔻��(���C����p) */
	/* ====================== */
	// �@�����蔻��(���C����)�p�̏����쐬
	KdCollider::RayInfo rayInfo;
	rayInfo.m_pos = GetPos();				// ���C�̔��ˈʒu��ݒ�
	rayInfo.m_dir = Math::Vector3::Down;	// ���C�̔��˕�����ݒ�
	rayInfo.m_type = KdCollider::TypeGround;// �����蔻����������^�C�v��ݒ�

	// ���������������΂�(�i���̋��e�͈�)
	rayInfo.m_pos.y += enableStepHigh;
	rayInfo.m_range = m_gravity + enableStepHigh;// ���C�̒�����ݒ�
	
	/* === �f�o�b�N�p === */
	m_debugWire.AddDebugLine(rayInfo.m_pos, rayInfo.m_dir, rayInfo.m_range);
 
	// �AHIT����ΏۃI�u�W�F�N�g�ɑ�������
	for (std::weak_ptr<KdGameObject>wpGameObj : m_wpHitObjList)
	{
		if (!wpGameObj.expired())
		{
			std::shared_ptr<KdGameObject> spGameObj = wpGameObj.lock();
			if (spGameObj)
			{
				std::list<KdCollider::CollisionResult> retRayList;
				spGameObj->Intersects(rayInfo, &retRayList);

				// �B���ʂ��g���č��W��⊮����
				// ���C�ɓ����������X�g�����ԋ߂��I�u�W�F�N�g�����o
				maxOverLap	= 0.0f;
				hit			= false;
				hitPos		= Math::Vector3::Zero;
				for (auto& ret : retRayList)
				{
					// ���C���Ւf���I�[�o�[������������Ԓ������̂�T��
					if (maxOverLap < ret.m_overlapDistance)
					{
						maxOverLap	= ret.m_overlapDistance;
						hitPos		= ret.m_hitPos;
						hit			= true;
					}
				}
				// ��������ɓ������Ă���
				if (hit)
				{
					SetPos(hitPos);
					m_gravity = 0;
				}
			}
		}
	}

	/* ==================== */
	/* �����蔻��(������p) */
	/* ==================== */
	// �@�����蔻��(������)�p�̏����쐬
	KdCollider::SphereInfo sphereInfo;
	sphereInfo.m_sphere.Center = GetPos() + Math::Vector3(0, 0.5f, 0);
	sphereInfo.m_sphere.Radius = 0.5f;
	sphereInfo.m_type = KdCollider::TypeBump;

	/* === �f�o�b�N�p(��) === */
	m_debugWire.AddDebugSphere(sphereInfo.m_sphere.Center, sphereInfo.m_sphere.Radius);

	// �AHIT����ΏۃI�u�W�F�N�g�ɑ�������
	for (std::weak_ptr<KdGameObject>wpGameObj : m_wpHitObjList)
	{
		if (!wpGameObj.expired())
		{
			std::shared_ptr<KdGameObject> spGameObj = wpGameObj.lock();
			if (spGameObj)
			{
				std::list<KdCollider::CollisionResult> retBumpList;
				spGameObj->Intersects(sphereInfo, &retBumpList);

				for (auto& ret : retBumpList)
				{
					Math::Vector3 newPos =
						GetPos() + (ret.m_hitDir * ret.m_overlapDistance);
					SetPos(newPos);
				}
				// �B���ʂ��g���č��W��⊮����
			}
		}
	}
}
