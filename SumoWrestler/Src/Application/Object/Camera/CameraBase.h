#pragma once

class CameraBase :public KdGameObject
{
public:
	CameraBase() { Init(); }
	~CameraBase() {}

	void Update();
	void PreDraw();
	void Init();

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
	// �J�����ϐ�(��]�����Ȃ��I)
	std::shared_ptr<KdCamera>	m_spCamera = nullptr;
	std::weak_ptr<KdGameObject>	m_wpTarget;

	Math::Matrix	m_localMat;
	Math::Matrix	m_rot;

	// �J������]�p�p�x
	Math::Vector3	m_degAng;
};
