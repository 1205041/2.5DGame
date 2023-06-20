#pragma once

class CameraBase :public KdGameObject
{
public:
	CameraBase() { Init(); }
	~CameraBase() {}

	void Init()		override;
	void Update()	override;
	void PreDraw()	override;

	void SetTarget(const std::shared_ptr<KdGameObject>& target);

	const std::shared_ptr<KdCamera>& GetCamera() { return m_spCamera; }
	const std::shared_ptr<KdCamera>& WorkCamera() { return m_spCamera; }

	const Math::Matrix GetRotationMatrix() const
	{
		return	Math::Matrix::CreateFromYawPitchRoll(
			DirectX::XMConvertToRadians(m_degAng.y),
			DirectX::XMConvertToRadians(m_degAng.x),
			DirectX::XMConvertToRadians(m_degAng.z)
		);
	}

	const Math::Matrix GetRotationYMatrix() const
	{
		return	Math::Matrix::CreateRotationY(
			DirectX::XMConvertToRadians(m_degAng.y));
	}

protected:
	// ƒJƒƒ‰•Ï”(‰ñ“]‚³‚¹‚È‚¢I)
	std::shared_ptr<KdCamera>	m_spCamera = nullptr;
	std::weak_ptr<KdGameObject>	m_wpTarget;

	Math::Matrix	m_localMat;
	Math::Matrix	m_rotation;

	// ƒJƒƒ‰‰ñ“]—pŠp“x
	Math::Vector3	m_degAng;
};
