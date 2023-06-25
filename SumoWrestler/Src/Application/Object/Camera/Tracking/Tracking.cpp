#include "Tracking.h"

void Tracking::Init()
{
	// ��_(�^�[�Q�b�g)����ǂꂾ������Ă��邩
	transMat = Math::Matrix::CreateTranslation(0, 0, -1.5f);

	// �ǂꂾ���X���Ă��邩/* ���Ԃ�Y,X,Z */
	rotMat = Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(10));

	CameraBase::Init();
}

void Tracking::Update()
{
	// �^�[�Q�b�g�̍s��(�L���ȏꍇ���p����)
	Math::Matrix targetMat = Math::Matrix::Identity;
	if (!m_wpTarget.expired())
	{
		std::shared_ptr<const KdGameObject> spTarget = m_wpTarget.lock();
		if (spTarget)
		{
			targetMat = spTarget->GetMatrix();
		}
	}

	m_mWorld = rotMat * transMat * targetMat;

	CameraBase::Update();
}