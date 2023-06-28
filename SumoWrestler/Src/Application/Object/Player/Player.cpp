#include "Player.h"

#include "../Camera/TPS/TPS.h"

// 更新関数
void Player::Update()
{
	m_spPoly->SetUVRect(Walk[(int)m_anime]);
	
	if (GetPos().y < -10) { m_isExpired = true; }
	if (m_isExpired)
	{
		KdAudioManager::Instance().StopAllSound();
		SceneManager::Instance().SetNextScene(SceneManager::SceneType::Lose);
	}

	m_gravity += 0.01f;
	m_mWorld._42 -= m_gravity;

	// キャラクターの移動速度
	m_nowPos  = GetPos();

	m_moveVec = Math::Vector3::Zero;
	if (GetAsyncKeyState('D') & 0x8000) 
	{
		m_moveVec.x = 1.0f;
		m_anime += 0.1f;
	}
	if (GetAsyncKeyState('A') & 0x8000) 
	{
		m_moveVec.x = -1.0f;
		m_anime += 0.1f;
	}
	if (GetAsyncKeyState('W') & 0x8000) 
	{
		m_moveVec.z = 1.0f;
		m_anime += 0.1f;
	}
	if (GetAsyncKeyState('S') & 0x8000) 
	{ 
		m_moveVec.z = -1.0f;
		m_anime += 0.1f;
	}

	// カメラ情報
	std::shared_ptr<CameraBase> spCamera = m_wpCamera.lock();
	if (spCamera)
	{
		m_moveVec = m_moveVec.TransformNormal
		(m_moveVec, spCamera->GetRotationYMatrix());
	}

	m_moveVec.Normalize();
	m_moveVec *= m_moveSpd;

	m_nowPos += m_moveVec;

	UpdateRotate(m_moveVec);

	if (m_anime >= 4) { m_anime = 0; }
}

// 更新後更新関数
void Player::PostUpdate()
{
	// キャラクターのワールド行列を創る処理
	Math::Matrix rotation = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_worldRot.y));
//	m_mWorld = rotation * Math::Matrix::CreateTranslation(m_nowPos);
	m_mWorld = Math::Matrix::CreateTranslation(m_nowPos);
	
	UpdateCollision();
}

// 影描画関数
void Player::GenerateDepthMapFromLight()
{
	if (!m_spPoly) { return; }
	KdShaderManager::Instance().m_HD2DShader.DrawPolygon(*m_spPoly, m_mWorld);
}

// 板ポリ描画関数
void Player::DrawLit()
{
	if (!m_spPoly) { return; }
	KdShaderManager::Instance().m_HD2DShader.DrawPolygon(*m_spPoly, m_mWorld);
}

// 初期化関数
void Player::Init()
{
	// キャラ初期値
	if (!m_spPoly)
	{
		m_spPoly = std::make_shared<KdSquarePolygon>();
		m_spPoly->SetMaterial(KdAssets::Instance().m_textures.GetData("Asset/Textures/Player/player.png"));

		m_spPoly->SetPivot(KdSquarePolygon::PivotType::Center_Bottom);
		m_spPoly->SetSplit(4, 4);
	}

	m_moveSpd = 0.05f;

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("PlayerCollider", GetPos(), 0.25f, KdCollider::TypeBump);
}

void Player::UpdateRotate(const Math::Vector3& _srcMoveVec)
{
	// 何も入力が無い場合は処理しない
	if (_srcMoveVec.LengthSquared() == 0.0f) { return; }

	// 移動方向のベクトル
	Math::Vector3 targetDir = _srcMoveVec;

	// 今向いている矢印情報が手に入る
	Math::Vector3 nowDir = GetMatrix().Backward();

	targetDir.Normalize();
	nowDir.Normalize();

	// キャラの向いている角度
	float nowAng = atan2(nowDir.x, nowDir.z);
	nowAng = DirectX::XMConvertToDegrees(nowAng);

	// 移動方向の角度
	float targetAng = atan2(targetDir.x, targetDir.z);
	targetAng = DirectX::XMConvertToDegrees(targetAng);

	// targetAngとnowAngの間の角度
	float betweenAng = targetAng - nowAng; // 差分を求める
	// 補正
	if (betweenAng > 180) { betweenAng -= 360; }
	else if (betweenAng < -180) { betweenAng += 360; }

	//
	float rotateAng = std::clamp(betweenAng, -8.0f, 8.0f);
	m_worldRot.y += rotateAng;
}

// 当たり判定関数
void Player::UpdateCollision()
{
	/* ====================== */
	/* 当たり判定(レイ判定用) */
	/* ====================== */
	// ①当たり判定(レイ判定)用の情報を作成
	KdCollider::RayInfo rayInfo;
	rayInfo.m_pos = GetPos();				// レイの発射位置を設定
	rayInfo.m_dir = Math::Vector3::Down;	// レイの発射方向を設定
	rayInfo.m_type = KdCollider::TypeGround;// 当たり判定をしたいタイプを設定

	// 少し高い所から飛ばす(段差の許容範囲)
	rayInfo.m_pos.y += enableStepHigh;
	rayInfo.m_range = m_gravity + enableStepHigh;// レイの長さを設定

	/* === デバック用 === */
//	m_debugWire.AddDebugLine(rayInfo.m_pos, rayInfo.m_dir, rayInfo.m_range);

	// ②HIT判定対象オブジェクトに総当たり
	for (std::weak_ptr<KdGameObject>wpGameObj : m_wpHitObjList)
	{
		if (!wpGameObj.expired())
		{
			std::shared_ptr<KdGameObject> spGameObj = wpGameObj.lock();
			if (spGameObj)
			{
				std::list<KdCollider::CollisionResult> retRayList;
				spGameObj->Intersects(rayInfo, &retRayList);

				// ③結果を使って座標を補完する
				// レイに当たったリストから一番近いオブジェクトを検出
				maxOverLap	= 0.0f;
				hit			= false;
				hitPos		= Math::Vector3::Zero;
				for (auto& ret : retRayList)
				{
					// レイを遮断しオーバーした長さが一番長いものを探す
					if (maxOverLap < ret.m_overlapDistance)
					{
						maxOverLap	= ret.m_overlapDistance;
						hitPos		= ret.m_hitPos;
						hit			= true;
					}
				}
				// 何かしらに当たっている
				if (hit)
				{
					SetPos(hitPos);
					m_gravity = 0;
				}
			}
		}
	}

	/* ==================== */
	/* 当たり判定(球判定用) */
	/* ==================== */
	// ①当たり判定(球判定)用の情報を作成
	KdCollider::SphereInfo sphereInfo;
	sphereInfo.m_sphere.Center = GetPos() + Math::Vector3(0, 0.5f, 0);
	sphereInfo.m_sphere.Radius = 0.5f;
	sphereInfo.m_type = KdCollider::TypeBump;

	/* === デバック用(球) === */
//	m_debugWire.AddDebugSphere(sphereInfo.m_sphere.Center, sphereInfo.m_sphere.Radius);

	// ②HIT判定対象オブジェクトに総当たり
	for (std::weak_ptr<KdGameObject>wpGameObj : m_wpHitObjList)
	{
		if (!wpGameObj.expired())
		{
			std::shared_ptr<KdGameObject> spGameObj = wpGameObj.lock();
			if (spGameObj)
			{
				std::list<KdCollider::CollisionResult> retBumpList;
				spGameObj->Intersects(sphereInfo, &retBumpList);

				for (auto& ret : retBumpList)
				{
					Math::Vector3 newPos = GetPos() + (ret.m_hitDir * ret.m_overlapDistance);
					SetPos(newPos);
				}
				// ③結果を使って座標を補完する
			}
		}
	}
}
