#pragma once

class SceneBase
{
public:
	SceneBase() { Init(); }
	~SceneBase() {}

	void PreUpdate();
	void Update();
	void PostUpdate();

	void PreDraw();
	void DrawLit();
	void DrawDebug();

	void DrawSprite();

	/* ゲッター */
	// オブジェクトリストを取得
	const std::list<std::shared_ptr<KdGameObject>>& GetObjList() { return m_objList; }
	const Math::Matrix GetRotationMatrix() const
	{
		/* 順番はY,X,Z */
		return Math::Matrix::CreateFromYawPitchRoll
		(
			DirectX::XMConvertToRadians(m_degAng.y),
			DirectX::XMConvertToRadians(m_degAng.x),
			DirectX::XMConvertToRadians(m_degAng.z)
		);
	}

	/* セッター */
	void AddObject(const std::shared_ptr<KdGameObject>& _obj) { m_objList.push_back(_obj); }
	void SetTarget(const std::shared_ptr<KdGameObject>& _target);

protected:
	virtual void Init() { m_degAng = Math::Vector3::Zero; }
	virtual void Event() {}

	void UpdateRotateByMouse();

	// オブジェクトリスト
	std::list<std::shared_ptr<KdGameObject>> m_objList;

	// カメラ変数(回転させない！)
	std::unique_ptr<KdCamera>			m_upCamera = nullptr;
	std::weak_ptr<const KdGameObject>	m_wpTarget;

	Math::Matrix	m_rotMat	= Math::Matrix::Identity;
	Math::Vector3	m_degAng	= Math::Vector3::Zero;

	POINT m_FixMousePos;
};
