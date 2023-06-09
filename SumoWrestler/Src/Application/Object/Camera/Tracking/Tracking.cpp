#include "Tracking.h"

void Tracking::Init()
{
	// 基準点(ターゲット)からどれだけ離れているか
	transMat = Math::Matrix::CreateTranslation(0, 0.5f, -1.0f);
	
	// どれだけ傾けているか/* 順番はY,X,Z */
	rotMat = Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(25));
	
	m_mWorld = rotMat * transMat;

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

	m_mWorld = rotMat * transMat * targetMat;

	CameraBase::Update();
}