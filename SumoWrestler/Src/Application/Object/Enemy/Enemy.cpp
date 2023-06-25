#include "Enemy.h"

void Enemy::Update()
{
	// 期限切れを有効にする
	if (GetPos().y < -10) { m_isExpired = true; }

	// デバック用
	m_debugWire.AddDebugSphere(GetPos() + Math::Vector3(0, 0.5f, 0), 0.2f, { 0,1,0,1 });

	m_nowPos = GetPos();
	m_nowPos.x += m_moveVec.x;
	m_nowPos.y += m_moveVec.y;
	m_nowPos.y -= m_gravity;
	m_gravity += 0.005f;

	// 左右移動
	++m_cnt;
	if (m_cnt > 120)
	{
		m_moveVec.x *= -1;
		m_cnt = 0;
	}

	// アニメーション
	int Walk[4] = { 3,4,3,5 };
	m_spPoly->SetUVRect(Walk[(int)m_anime]);
	m_anime += 0.1f;
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
		m_spPoly->SetMaterial(KdAssets::Instance().m_textures.GetData("Asset/Textures/Player/char.png"));

		// アニメーション
		m_spPoly->SetPivot(KdSquarePolygon::PivotType::Center_Bottom);
		m_spPoly->SetSplit(6, 6);
	}
	m_anime = 0;

	SetPos({ 3.0f,-3.0f,0 });
	m_moveVec = Math::Vector3::Zero;
}

void Enemy::UpdateCollision()
{
	/* ====================== */
	/* 当たり判定(レイ判定用) */
	/* ====================== */
	KdCollider::RayInfo rayInfo;		// レイの発射位置を設定
	rayInfo.m_pos = GetPos();			//GetPos()はキャラの足元のはず！
	rayInfo.m_dir = Math::Vector3::Down;// レイの発射方向を設定
	rayInfo.m_pos.y += 0.1f;			// 少し高い所から飛ばす

	// 段差の許容範囲
	rayInfo.m_pos.y += enableStepHigh;
	rayInfo.m_range = m_gravity + enableStepHigh;	// レイの長さを設定
	rayInfo.m_type = KdCollider::TypeGround;		// 当たり判定をしたいタイプを設定

	/* === デバック用 === */
	m_debugWire.AddDebugLine(rayInfo.m_pos, rayInfo.m_dir, rayInfo.m_range);

	// レイに当たったオブジェクト情報を格納するリスト
	std::list<KdCollider::CollisionResult> retRayList;

	/* レイと当たり判定をする */
	for (auto& obj : SceneManager::Instance().GetObjList()) { obj->Intersects(rayInfo, &retRayList); }

	// レイに当たったリストから一番近いオブジェクトを検出
	maxOverLap	= 0.0f;
	hit			= false;
	groundPos	= Math::Vector3::Zero;
	for (auto& ret : retRayList)
	{
		// レイを遮断してオーバーした長さが一番長いものを探す
		if (maxOverLap < ret.m_overlapDistance)
		{
			maxOverLap	= ret.m_overlapDistance;
			groundPos	= ret.m_hitPos;
			hit			= true;
		}
	}
	if (hit)
	{
		// 地面に当たっている
		Math::Vector3	hitPos =
			groundPos + Math::Vector3(0, -0.1f, 0);
		SetPos(hitPos);
		m_gravity = 0;
	}
	else
	{
		notHitCnt++;
		if (notHitCnt >= 60)
		{
			SceneManager::Instance().SetNextScene(SceneManager::SceneType::Win);
		}
	}

	/* ==================== */
	/* 当たり判定(球判定用) */
	/* ==================== */
	KdCollider::SphereInfo sphereInfo;		// 球判定用の変数
	sphereInfo.m_sphere.Center = GetPos() + Math::Vector3(0, 0.5f, 0);	// 球の中心位置を設定
	sphereInfo.m_sphere.Radius = 0.3f;									// 球の半径を設定
	sphereInfo.m_type = KdCollider::TypeDamage;		// 当たり判定をしたいタイプを設定

	/* === デバック用(球) === */
	m_debugWire.AddDebugSphere(sphereInfo.m_sphere.Center, sphereInfo.m_sphere.Radius);

	// 球に当たったオブジェクト情報を格納するリスト
	std::list<KdCollider::CollisionResult> retSphereList;

	// 球と当たり判定
	for (auto& obj : SceneManager::Instance().GetObjList()) { obj->Intersects(sphereInfo, &retSphereList); }

	// 球に当たったリストから一番近いオブジェクトを検出
	maxOverLap	= 0.0f;
	hit			= false;
	Math::Vector3 hitDir = Math::Vector3::Zero;// 当たっている方向
	for (auto& ret : retSphereList)
	{
		// 一番めり込んだオブジェクトを探す
		if (maxOverLap < ret.m_overlapDistance)
		{
			maxOverLap	= ret.m_overlapDistance;
			hitDir		= ret.m_hitDir;// 押し返す方向
			hit			= true;
		}
	}
	if (hit)
	{
		// 押し返し
		Math::Vector3 newPos =
			GetPos() + (hitDir * maxOverLap);
		SetPos(newPos);
	}
}
