#pragma once
#include "../ObjBase.h"

class CameraBase :public ObjBase
{
public:
	CameraBase() { Init(); }
	~CameraBase() {}

	void Init()		override;
	void Update()	override;
	void PreDraw()	override;

	void SetTarget(const std::shared_ptr<KdGameObject>& _target);

	const std::shared_ptr<KdCamera>& GetCamera() { return m_spCamera; }
	const std::shared_ptr<KdCamera>& WorkCamera() { return m_spCamera; }

	// m_rotの補正
	const Math::Matrix GetRotationMatrix() const
	{
		return	Math::Matrix::CreateFromYawPitchRoll(
			DirectX::XMConvertToRadians(m_degAng.y),
			DirectX::XMConvertToRadians(m_degAng.x),
			DirectX::XMConvertToRadians(m_degAng.z)
		);
	}

	// プレイヤーのカメラ情報
	const Math::Matrix GetRotationYMatrix() const
	{
		return	Math::Matrix::CreateRotationY(
			DirectX::XMConvertToRadians(m_degAng.y));
	}

protected:
	// カメラ変数(回転させない！)
	std::shared_ptr<KdCamera>			m_spCamera = nullptr;
	std::weak_ptr<const KdGameObject>	m_wpTarget;

	Math::Matrix	m_localMat;
	Math::Matrix	m_rot;

	// カメラ回転用角度
	Math::Vector3	m_degAng;
};
