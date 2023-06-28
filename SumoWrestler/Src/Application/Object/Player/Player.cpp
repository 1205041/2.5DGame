#include "Player.h"

#include "../Camera/TPS/TPS.h"

// �X�V�֐�
void Player::Update()
{
	m_spPoly->SetUVRect(Walk[(int)m_anime]);
	
	if (GetPos().y < -10) { m_isExpired = true; }
	if (m_isExpired)
	{
		KdAudioManager::Instance().StopAllSound();
		SceneManager::Instance().SetNextScene(SceneManager::SceneType::Lose);
	}

	m_gravity += 0.01f;
	m_mWorld._42 -= m_gravity;

	// �L�����N�^�[�̈ړ����x
	m_nowPos  = GetPos();

	m_moveVec = Math::Vector3::Zero;
	if (GetAsyncKeyState('D') & 0x8000) 
	{
		m_moveVec.x = 1.0f;
		m_anime += 0.1f;
	}
	if (GetAsyncKeyState('A') & 0x8000) 
	{
		m_moveVec.x = -1.0f;
		m_anime += 0.1f;
	}
	if (GetAsyncKeyState('W') & 0x8000) 
	{
		m_moveVec.z = 1.0f;
		m_anime += 0.1f;
	}
	if (GetAsyncKeyState('S') & 0x8000) 
	{ 
		m_moveVec.z = -1.0f;
		m_anime += 0.1f;
	}

	// �J�������
	std::shared_ptr<CameraBase> spCamera = m_wpCamera.lock();
	if (spCamera)
	{
		m_moveVec = m_moveVec.TransformNormal
		(m_moveVec, spCamera->GetRotationYMatrix());
	}

	m_moveVec.Normalize();
	m_moveVec *= m_moveSpd;

	m_nowPos += m_moveVec;

	UpdateRotate(m_moveVec);

	if (m_anime >= 4) { m_anime = 0; }
}

// �X�V��X�V�֐�
void Player::PostUpdate()
{
	// �L�����N�^�[�̃��[���h�s���n�鏈��
	Math::Matrix rotation = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_worldRot.y));
//	m_mWorld = rotation * Math::Matrix::CreateTranslation(m_nowPos);
	m_mWorld = Math::Matrix::CreateTranslation(m_nowPos);
	
	UpdateCollision();
}

// �e�`��֐�
void Player::GenerateDepthMapFromLight()
{
	if (!m_spPoly) { return; }
	KdShaderManager::Instance().m_HD2DShader.DrawPolygon(*m_spPoly, m_mWorld);
}

// �|���`��֐�
void Player::DrawLit()
{
	if (!m_spPoly) { return; }
	KdShaderManager::Instance().m_HD2DShader.DrawPolygon(*m_spPoly, m_mWorld);
}

// �������֐�
void Player::Init()
{
	// �L���������l
	if (!m_spPoly)
	{
		m_spPoly = std::make_shared<KdSquarePolygon>();
		m_spPoly->SetMaterial(KdAssets::Instance().m_textures.GetData("Asset/Textures/Player/player.png"));

		m_spPoly->SetPivot(KdSquarePolygon::PivotType::Center_Bottom);
		m_spPoly->SetSplit(4, 4);
	}

	m_moveSpd = 0.05f;

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("PlayerCollider", GetPos(), 0.25f, KdCollider::TypeBump);
}

void Player::UpdateRotate(const Math::Vector3& _srcMoveVec)
{
	// �������͂������ꍇ�͏������Ȃ�
	if (_srcMoveVec.LengthSquared() == 0.0f) { return; }

	// �ړ������̃x�N�g��
	Math::Vector3 targetDir = _srcMoveVec;

	// �������Ă������񂪎�ɓ���
	Math::Vector3 nowDir = GetMatrix().Backward();

	targetDir.Normalize();
	nowDir.Normalize();

	// �L�����̌����Ă���p�x
	float nowAng = atan2(nowDir.x, nowDir.z);
	nowAng = DirectX::XMConvertToDegrees(nowAng);

	// �ړ������̊p�x
	float targetAng = atan2(targetDir.x, targetDir.z);
	targetAng = DirectX::XMConvertToDegrees(targetAng);

	// targetAng��nowAng�̊Ԃ̊p�x
	float betweenAng = targetAng - nowAng; // ���������߂�
	// �␳
	if (betweenAng > 180) { betweenAng -= 360; }
	else if (betweenAng < -180) { betweenAng += 360; }

	//
	float rotateAng = std::clamp(betweenAng, -8.0f, 8.0f);
	m_worldRot.y += rotateAng;
}

// �����蔻��֐�
void Player::UpdateCollision()
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
//	m_debugWire.AddDebugLine(rayInfo.m_pos, rayInfo.m_dir, rayInfo.m_range);

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
//	m_debugWire.AddDebugSphere(sphereInfo.m_sphere.Center, sphereInfo.m_sphere.Radius);

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
					Math::Vector3 newPos = GetPos() + (ret.m_hitDir * ret.m_overlapDistance);
					SetPos(newPos);
				}
				// �B���ʂ��g���č��W��⊮����
			}
		}
	}
}
