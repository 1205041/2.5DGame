#include "TPS.h"

void TPS::Init()
{
	// 基準点(ターゲット)の目線の位置
	m_localMat = Math::Matrix::CreateTranslation(Math::Vector3(0.0f, 1.0f, -5.0f));

	// カーソル初期化
	m_FixMousePos.x = 640;
	m_FixMousePos.y = 360;

	SetCursorPos(m_FixMousePos.x, m_FixMousePos.y);

	CameraBase::Init();
}

void TPS::Update()
{
	// ターゲットの行列
	Math::Matrix targetMat = Math::Matrix::Identity;
	if (!m_wpTarget.expired())
	{
		const std::shared_ptr<KdGameObject>& spTarget = m_wpTarget.lock();
		if (spTarget)
		{
			targetMat = Math::Matrix::CreateTranslation(spTarget->GetPos());
		}
	}

	// 行列合成
	UpdateRotateByMouse();
	m_rot = GetRotationMatrix();
	m_mWorld = m_localMat * m_rot * targetMat;

	CameraBase::Update();
}

void TPS::UpdateRotateByMouse()
{
	// マウス位置の差分を得る
	POINT nowPos;
	GetCursorPos(&nowPos);

	POINT mouseMove;
	mouseMove.x = nowPos.x - m_FixMousePos.x;
	mouseMove.y = nowPos.y - m_FixMousePos.y;

	SetCursorPos(m_FixMousePos.x, m_FixMousePos.y);

	// カメラを回転させる為に各軸の回転角度を設定する
	m_degAng.x += mouseMove.y * 0.15f;
	m_degAng.y += mouseMove.x * 0.15f;

	// 回転制御
	m_degAng.x = std::clamp(m_degAng.x, -FLT_MAX, FLT_MAX);
}
