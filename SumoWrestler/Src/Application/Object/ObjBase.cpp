#include "ObjBase.h"

void ObjBase::UpdateCollision()
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
				maxOverLap = 0.0f;
				hit = false;
				hitPos = Math::Vector3::Zero;
				for (auto& ret : retRayList)
				{
					// レイを遮断しオーバーした長さが一番長いものを探す
					if (maxOverLap < ret.m_overlapDistance)
					{
						maxOverLap = ret.m_overlapDistance;
						hitPos = ret.m_hitPos;
						hit = true;
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
					Math::Vector3 newPos =
						GetPos() + (ret.m_hitDir * ret.m_overlapDistance);
					SetPos(newPos);
				}
				// ③結果を使って座標を補完する
			}
		}
	}
}

float ObjBase::GetAngleDeg(Math::Vector3 _src, Math::Vector3 _dest)
{
	//手順1. 横方向の距離aと縦方向の距離bを減算で求める
	float a = _dest.x - _src.x;
	float b = _dest.z - _src.z;

	//手順2. atan2 関数を活用して角度を求める
	float rad = atan2(b, a);

	//手順3. 補正処理:必要に応じて、デグリー(度)に直す
	float deg = DirectX::XMConvertToDegrees(rad);

	//手順4.0～360に補正
	if (deg < 0) { deg += 360; }

	return deg;
}
