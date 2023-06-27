#pragma once
#include "../ObjBase.h"

class CameraBase;

class Player :public ObjBase
{
public:
	Player() { Init(); }
	~Player() {}

	void Update()					override;
	void PostUpdate()				override;

	void GenerateDepthMapFromLight()override;
	void DrawLit()					override;

	void Init()						override;

	void SetCamera(const std::shared_ptr<CameraBase>& _camera) { m_wpCamera = _camera; }
private:
	// 衝突判定とそれに伴う座標の更新
	void UpdateCollision();

	void UpdateRotate(const Math::Vector3& _srcMoveVec);

	std::weak_ptr<CameraBase> m_wpCamera;

	// ワールド行列を作る為のそれぞれの回転角度
	Math::Vector3	m_worldRot;

	int m_cnt = 0;
};