#include "Player.h"

#include "../Camera/TPS/TPS.h"

void Player::Update()
{
	m_gravity += 0.01f;
	m_mWorld._42 -= m_gravity;

	// �L�����N�^�[�̈ړ����x
	m_nowPos  = GetPos();

	m_moveVec = Math::Vector3::Zero;
	if (GetAsyncKeyState('D') & 0x8000) { m_moveVec.x = 1.0f; }
	if (GetAsyncKeyState('A') & 0x8000) { m_moveVec.x = -1.0f; }
	if (GetAsyncKeyState('W') & 0x8000) { m_moveVec.z = 1.0f; }
	if (GetAsyncKeyState('S') & 0x8000) { m_moveVec.z = -1.0f; }
//	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
//	{
//		if (!m_pushFlg)
//		{
//			m_pushFlg = true;
//		}
//	}
//	else
//	{
//		m_pushFlg = false;
//	}

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

	// �L�����̃A�j���[�V����
	// 24,25,26��24,25,24,26
	int Run[4] = { 24,25,24,26 };
	m_spPoly->SetUVRect(Run[(int)m_anime]);
	m_anime += 0.2f;
	if (m_anime >= 4) { m_anime = 0; }
}

void Player::PostUpdate()
{
	// �L�����N�^�[�̃��[���h�s���n�鏈��
	Math::Matrix rotation = Math::Matrix::CreateRotationY
	(DirectX::XMConvertToRadians(m_worldRot.y));
	m_mWorld = Math::Matrix::CreateTranslation(m_nowPos);
	
	UpdateCollision();
}

void Player::GenerateDepthMapFromLight()
{
	// �|��(�e)
	if (!m_spPoly) { return; }

	KdShaderManager::Instance().m_HD2DShader.DrawPolygon(*m_spPoly, m_mWorld);
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
		m_spPoly->SetMaterial(KdAssets::Instance().m_textures.GetData("Asset/Textures/Player/char.png"));

		m_spPoly->SetPivot(KdSquarePolygon::PivotType::Center_Bottom);
		m_spPoly->SetSplit(6, 6);
	}

	m_moveSpd = 0.05f;

	// �A�j���[�V����������
	m_anime = 0.0f;

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("PlayerCollider", GetPos(), 0.3f, KdCollider::TypeBump);
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
			SceneManager::Instance().SetNextScene(SceneManager::SceneType::Lose);
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
			maxOverLap = ret.m_overlapDistance;
			hit = true;
			hitDir = ret.m_hitDir;// �����Ԃ�����
		}
	}
	if (hit)
	{
		Math::Vector3 newPos =
			GetPos() + (hitDir * maxOverLap);
		SetPos(newPos);
	}
}
