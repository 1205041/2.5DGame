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
	
	void RegistHitObj(const std::shared_ptr<KdGameObject>& _obj) { m_wpHitObjList.push_back(_obj); }

private:
	void UpdateRotate(Math::Vector3& _srcMoveVec);

	// 衝突判定とそれに伴う座標の更新
	void UpdateCollision();

	std::weak_ptr<CameraBase>					m_wpCamera;

	// 当たり判定リスト
	std::vector<std::weak_ptr<KdGameObject>>	m_wpHitObjList;
	
	// キャラクターの移動速度
	float			m_moveSpd = 0.0f;

	// ワールド行列を作る為のそれぞれの回転角度
	Math::Vector3	m_worldRot;
	
	bool			m_pushFlg = false;
};