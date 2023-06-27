#include "Enemy.h"
#include "../Player/Player.h"

void Enemy::Update()
{
	// 期限切れを有効にする
	if (GetPos().y < -10) { m_isExpired = true; }
	if(m_isExpired)
	{
		KdAudioManager::Instance().StopAllSound();
		SceneManager::Instance().SetNextScene(SceneManager::SceneType::Win);
	}

	m_gravity += 0.01f;
	m_mWorld._42 -= m_gravity;

	// 追従移動
	m_nowPos = GetPos();

	m_moveVec = Math::Vector3::Zero;
	
	m_moveVec.Normalize();
	m_moveVec *= m_moveSpd;

	m_nowPos += m_moveVec;

	// アニメーション
	int Walk[4] = { 8,9,8,10 };
	m_spPoly->SetUVRect(Walk[(int)m_anime]);
	m_anime += 0.05f;
	if (m_anime >= 4) { m_anime = 0; }
}

void Enemy::PostUpdate()
{
	Math::Matrix transMat;
	// キャラの座標行列
	transMat = Math::Matrix::CreateTranslation(m_nowPos);
	m_mWorld = transMat;
	
	UpdateCollision();
}

void Enemy::GenerateDepthMapFromLight()
{
	// 板ポリ(影)
	if (!m_spPoly) { return; }

	KdShaderManager::Instance().m_HD2DShader.DrawPolygon(*m_spPoly, m_mWorld);
}

void Enemy::DrawLit()
{
	// 板ポリ(キャラ)
	if (!m_spPoly) { return; }

	KdShaderManager::Instance().m_HD2DShader.DrawPolygon(*m_spPoly, m_mWorld);
}

void Enemy::Init()
{
	// エネミー初期化
	if (!m_spPoly)
	{
		m_spPoly = std::make_shared<KdSquarePolygon>();
		m_spPoly->SetMaterial(KdAssets::Instance().m_textures.GetData("Asset/Textures/Enemy/enemy.png"));

		// アニメーション
		m_spPoly->SetPivot(KdSquarePolygon::PivotType::Center_Bottom);
		m_spPoly->SetSplit(4, 4);
	}
	
	m_moveSpd = 0.05f;
	m_moveVec = Math::Vector3::Zero;

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("EnemyCollider", GetPos(), 0.25f, KdCollider::TypeBump);
}

void Enemy::UpdateCollision()
{
	/* ====================== */
	/* 当たり判定(レイ判定用) */
	/* ====================== */
	// �@当たり判定(レイ判定)用の情報を作成
	KdCollider::RayInfo rayInfo;
	rayInfo.m_pos = GetPos();				// レイの発射位置を設定
	rayInfo.m_dir = Math::Vector3::Down;	// レイの発射方向を設定
	rayInfo.m_type = KdCollider::TypeGround;// 当たり判定をしたいタイプを設定

	// 少し高い所から飛ばす(段差の許容範囲)
	rayInfo.m_pos.y += enableStepHigh;
	rayInfo.m_range = m_gravity + enableStepHigh;// レイの長さを設定
	
	/* === デバック用 === */
	m_debugWire.AddDebugLine(rayInfo.m_pos, rayInfo.m_dir, rayInfo.m_range);
 
	// �AHIT判定対象オブジェクトに総当たり
	for (std::weak_ptr<KdGameObject>wpGameObj : m_wpHitObjList)
	{
		if (!wpGameObj.expired())
		{
			std::shared_ptr<KdGameObject> spGameObj = wpGameObj.lock();
			if (spGameObj)
			{
				std::list<KdCollider::CollisionResult> retRayList;
				spGameObj->Intersects(rayInfo, &retRayList);

				// �B結果を使って座標を補完する
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
	// �@当たり判定(球判定)用の情報を作成
	KdCollider::SphereInfo sphereInfo;
	sphereInfo.m_sphere.Center = GetPos() + Math::Vector3(0, 0.5f, 0);
	sphereInfo.m_sphere.Radius = 0.5f;
	sphereInfo.m_type = KdCollider::TypeBump;

	/* === デバック用(球) === */
	m_debugWire.AddDebugSphere(sphereInfo.m_sphere.Center, sphereInfo.m_sphere.Radius);

	// �AHIT判定対象オブジェクトに総当たり
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
					Math::Vector3 newPos =
						GetPos() + (ret.m_hitDir * ret.m_overlapDistance);
					SetPos(newPos);
				}
				// �B結果を使って座標を補完する
			}
		}
	}
}
