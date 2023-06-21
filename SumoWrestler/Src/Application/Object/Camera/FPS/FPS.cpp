#include "FPS.h"

void FPS::Init()
{
	// 基準点(ターゲット)の目線の位置
	m_localMat = Math::Matrix::CreateTranslation(0.0f, 1.5f, 0.0f);

	m_FixMousePos.x = 640;
	m_FixMousePos.y = 360;

	SetCursorPos(m_FixMousePos.x, m_FixMousePos.y);

	CameraBase::Init();
}

void FPS::Update()
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

	// カメラの回転
	UpdateRotateByMouse();
	m_rot = GetRotationMatrix();
	m_mWorld = m_rot * m_localMat * targetMat;

	CameraBase::Update();
}

void FPS::UpdateRotateByMouse()
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
}
