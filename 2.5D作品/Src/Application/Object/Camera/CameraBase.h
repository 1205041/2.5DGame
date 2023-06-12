#pragma once

class CameraBase :public KdGameObject
{
public:
	CameraBase() { Init(); }
	~CameraBase() {}

	virtual void Init()   override;
	virtual void Update() override;
	virtual void PreDraw()override;

	void SetTaret(const std::shared_ptr<KdGameObject>& _target);

	std::shared_ptr<const KdCamera> WorkCamera() const { return m_spCamera; }

	const Math::Matrix GetRotationMatrix() const
	{
		/* èáî‘ÇÕY,X,Z */
		return Math::Matrix::CreateFromYawPitchRoll
		(
			DirectX::XMConvertToRadians(m_degAng.y),
			DirectX::XMConvertToRadians(m_degAng.x),
			DirectX::XMConvertToRadians(m_degAng.z)
		);
	}

	const Math::Matrix GetRotationYMatrix() const
	{
		/* èáî‘ÇÕY,X,Z */
		return Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_degAng.y));
	}
protected:
	std::shared_ptr<KdCamera> m_spCamera = nullptr;
	std::weak_ptr<const KdGameObject> m_wpTarget;

	Math::Matrix m_localPos;
	Math::Matrix m_rot;

	Math::Vector3 m_degAng;
};
