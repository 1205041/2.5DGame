#include "Player.h"

#include "../Camera/CameraBase.h"

void Player::Update()
{
	// �L�����̃A�j���[�V����
	// 24,25,26��24,25,24,26
	int Run[4] = { 24,25,24,26 };
	m_spPoly->SetUVRect(Run[(int)m_anime]);
	m_anime += 0.2f;
	if (m_anime >= 4) { m_anime = 0; }

	m_gravity += 0.01f;
	m_mWorld._42 -= m_gravity;

	// �L�����N�^�[�̈ړ����x
	moveSpd = 0.05f;
	nowPos = GetPos();

	m_moveVec = Math::Vector3::Zero;
	if (GetAsyncKeyState('D') & 0x8000) { m_moveVec.x = 1.0f; }
	if (GetAsyncKeyState('A') & 0x8000) { m_moveVec.x = -1.0f; }
	if (GetAsyncKeyState('W') & 0x8000) { m_moveVec.z = 1.0f; }
	if (GetAsyncKeyState('S') & 0x8000) { m_moveVec.z = -1.0f; }
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

	// �J�������
	std::shared_ptr<CameraBase> spCamera = m_wpCamera.lock();
	if (spCamera)
	{
		m_moveVec = m_moveVec.TransformNormal
		(m_moveVec, spCamera->GetRotationYMatrix());
	}

	m_moveVec.Normalize();
	m_moveVec *= moveSpd;

	nowPos.x += m_moveVec.x;
	nowPos.z += m_moveVec.z;

	UpdateRotate(m_moveVec);
	UpdateCollision();
}

void Player::PostUpdate()
{
	Math::Matrix rotation = Math::Matrix::CreateRotationY
	(DirectX::XMConvertToRadians(m_worldRot.y));

	m_mWorld = rotation * Math::Matrix::CreateTranslation(nowPos);
}

void Player::DrawLit()
{
	// �|��(�L����)
	if (!m_spPoly) { return; }

	KdShaderManager::Instance().m_HD2DShader.DrawPolygon(*m_spPoly, m_mWorld);
}

void Player::Init()
{
	// �L���������l
	if (!m_spPoly)
	{
		m_spPoly = std::make_shared<KdSquarePolygon>();
		m_spPoly->SetMaterial("Asset/Textures/char.png");

		m_spPoly->SetPivot(KdSquarePolygon::PivotType::Center_Bottom);
		m_spPoly->SetSplit(6, 6);
	}

	m_gravity = 0.0f;
	SetPos({ -3.0f,0,0 });

	// �A�j���[�V����������
	m_anime = 0.0f;
}

void Player::DrawDebug()
{
	m_debugWire.Draw();
}

void Player::UpdateRotate(Math::Vector3& _srcMoveVec)
{
	// �������͂������ꍇ�͏������Ȃ�
	/* �P���Ȓ����𑪂鎞��.LengthSquared()������ */
	if (_srcMoveVec.LengthSquared() == 0.0f) { return; }

	// �ړ������̃x�N�g��
	Math::Vector3 targetDir = _srcMoveVec;

	/* �I�I�I �L�����̐��ʕ����̃x�N�g�� �I�I�I */
	// �������Ă������񂪎�ɓ���
	Math::Vector3 nowDir = GetMatrix().Backward();
	//	Math::Vector3 nowDir = GetMatrix().Forward();

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

void Player::UpdateCollision()
{
	/* ====================== */
	/* �����蔻��(���C����p) */
	/* ====================== */
	// �@�����蔻��(���C����)�p�̏����쐬
	KdCollider::RayInfo rayInfo;
	rayInfo.m_pos = GetPos();						// ���C�̔��ˈʒu��ݒ�

	// ���������������΂�(�i���̋��e�͈�)
	static float enableStepHigh = 0.2f;
	rayInfo.m_pos.y += enableStepHigh;

	rayInfo.m_dir = Math::Vector3::Down;			// ���C�̔��˕�����ݒ�
	rayInfo.m_range = m_gravity + enableStepHigh;	// ���C�̒�����ݒ�
	rayInfo.m_type = KdCollider::TypeGround;		// �����蔻����������^�C�v��ݒ�

	/* === �f�o�b�N�p === */
	m_debugWire.AddDebugLine
	(
		rayInfo.m_pos,
		rayInfo.m_dir,
		rayInfo.m_range
	);

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
				float			maxOverLap = 0.0f;
				Math::Vector3	hitPos = Math::Vector3::Zero;
				bool			hit = false;
				for (auto& ret : retRayList)
				{
					// ���C���Ւf���I�[�o�[������������Ԓ������̂�T��
					if (maxOverLap < ret.m_overlapDistance)
					{
						maxOverLap = ret.m_overlapDistance;
						hitPos = ret.m_hitPos;
						hit = true;
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
