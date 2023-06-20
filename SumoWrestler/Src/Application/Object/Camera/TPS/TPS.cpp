#include "TPS.h"

void TPS::Init()
{
	// ��_(�^�[�Q�b�g)�̖ڐ��̈ʒu
	m_localMat = Math::Matrix::CreateTranslation(Math::Vector3(0.0f, 1.f, -10));

	// �J�[�\��������
	m_FixMousePos.x = 640;
	m_FixMousePos.y = 360;

	SetCursorPos(m_FixMousePos.x, m_FixMousePos.y);

	CameraBase::Init();
}

void TPS::Update()
{
	// �^�[�Q�b�g�̍s��
	Math::Matrix targetMat = Math::Matrix::Identity;
	if (!m_wpTarget.expired())
	{
		std::shared_ptr<const KdGameObject> spTarget = m_wpTarget.lock();
		if (spTarget)
		{
			targetMat = Math::Matrix::CreateTranslation(spTarget->GetPos());
		}
	}

	// �s�񍇐�
	UpdateRotateByMouse();
	m_rotation = GetRotationMatrix();
	m_mWorld = m_localMat * m_rotation * targetMat;

	CameraBase::Update();
}

void TPS::UpdateRotateByMouse()
{
	// �}�E�X�ʒu�̍����𓾂�
	POINT nowPos;
	GetCursorPos(&nowPos);

	POINT mouseMove;
	mouseMove.x = nowPos.x - m_FixMousePos.x;
	mouseMove.y = nowPos.y - m_FixMousePos.y;

	SetCursorPos(m_FixMousePos.x, m_FixMousePos.y);

	// �J��������]������ׂɊe���̉�]�p�x��ݒ肷��
	m_degAng.x += mouseMove.y * 0.15f;
	m_degAng.y += mouseMove.x * 0.15f;

	// ��]����
	m_degAng.x = std::clamp(m_degAng.x, -FLT_MAX, FLT_MAX);
}