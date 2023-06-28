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
