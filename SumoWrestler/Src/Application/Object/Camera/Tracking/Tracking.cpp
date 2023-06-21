#include "Tracking.h"

void Tracking::Init()
{
	// 基準点(ターゲット)からどれだけ離れているか
	m_localMat = Math::Matrix::CreateTranslation(0, 6.0f, -5.0f);

	// どれだけ傾けているか
	m_rot = Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(45));

	CameraBase::Init();
}

void Tracking::Update()
{
	// ターゲットの行列(有効な場合利用する)
	Math::Matrix targetMat = Math::Matrix::Identity;
	if (!m_wpTarget.expired())
	{
		std::shared_ptr<const KdGameObject> spTarget = m_wpTarget.lock();
		if (spTarget)
		{
			targetMat = spTarget->GetMatrix();
		}
	}

	m_mWorld = m_rot * m_localMat * targetMat;

	CameraBase::Update();
}