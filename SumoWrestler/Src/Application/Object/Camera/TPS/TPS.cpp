#include "TPS.h"

void TPS::Init()
{
	// 基準点(ターゲット)の目線の位置
	m_localMat = Math::Matrix::CreateTranslation(Math::Vector3(0.0f, 1.f, -10));

	// カーソル初期化
	m_FixMousePos.x = 640;
	m_FixMousePos.y = 360;

	SetCursorPos(m_FixMousePos.x, m_FixMousePos.y);
}

void TPS::Update()
{
	// ターゲットの行列
	Math::Matrix targetMat = Math::Matrix::Identity;
	if (!m_wpTarget.expired())
	{
		std::shared_ptr<const KdGameObject> spTarget = m_wpTarget.lock();
		if (spTarget)
		{
			targetMat = Math::Matrix::CreateTranslation(spTarget->GetPos());
		}
	}

	// 行列合成
	UpdateRotateByMouse();
	m_rotMat = GetRotationMatrix();
	Math::Matrix mat = m_localMat * m_rotMat * targetMat;
}

void TPS::UpdateRotateByMouse()
{
}
