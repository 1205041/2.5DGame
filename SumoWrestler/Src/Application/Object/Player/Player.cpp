#include "Player.h"

#include "../Camera/TPS/TPS.h"

void Player::Update()
{
	m_gravity += 0.01f;
	m_mWorld._42 -= m_gravity;

	// キャラクターの移動速度
	m_nowPos  = GetPos();

	m_moveVec = Math::Vector3::Zero;
	if (GetAsyncKeyState('D') & 0x8000) { m_moveVec.x = 1.0f; }
	if (GetAsyncKeyState('A') & 0x8000) { m_moveVec.x = -1.0f; }
	if (GetAsyncKeyState('W') & 0x8000) { m_moveVec.z = 1.0f; }
	if (GetAsyncKeyState('S') & 0x8000) { m_moveVec.z = -1.0f; }
//	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
//	{
//		if (!m_pushFlg)
//		{
//			m_pushFlg = true;
//		}
//	}
//	else
//	{
//		m_pushFlg = false;
//	}

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

	// キャラのアニメーション
	// 24,25,26→24,25,24,26
	int Run[4] = { 24,25,24,26 };
	m_spPoly->SetUVRect(Run[(int)m_anime]);
	m_anime += 0.2f;
	if (m_anime >= 4) { m_anime = 0; }
}

void Player::PostUpdate()
{
	// キャラクターのワールド行列を創る処理
	Math::Matrix rotation = Math::Matrix::CreateRotationY
	(DirectX::XMConvertToRadians(m_worldRot.y));
	m_mWorld = Math::Matrix::CreateTranslation(m_nowPos);
	
	UpdateCollision();
}

void Player::GenerateDepthMapFromLight()
{
	// 板ポリ(影)
	if (!m_spPoly) { return; }

	KdShaderManager::Instance().m_HD2DShader.DrawPolygon(*m_spPoly, m_mWorld);
}

void Player::DrawLit()
{
	// 板ポリ(キャラ)
	if (!m_spPoly) { return; }

	KdShaderManager::Instance().m_HD2DShader.DrawPolygon(*m_spPoly, m_mWorld);
}

void Player::Init()
{
	// キャラ初期値
	if (!m_spPoly)
	{
		m_spPoly = std::make_shared<KdSquarePolygon>();
		m_spPoly->SetMaterial(KdAssets::Instance().m_textures.GetData("Asset/Textures/Player/char.png"));

		m_spPoly->SetPivot(KdSquarePolygon::PivotType::Center_Bottom);
		m_spPoly->SetSplit(6, 6);
	}

	m_moveSpd = 0.05f;

	// アニメーション初期化
	m_anime = 0.0f;

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("PlayerCollider", GetPos(), 0.3f, KdCollider::TypeBump);
}

void Player::UpdateRotate(Math::Vector3& _srcMoveVec)
{
	// 何も入力が無い場合は処理しない
	/* 単純な長さを測る時は.LengthSquared()が速い */
	if (_srcMoveVec.LengthSquared() == 0.0f) { return; }

	// 移動方向のベクトル
	Math::Vector3 targetDir = _srcMoveVec;

	/* ！！！ キャラの正面方向のベクトル ！！！ */
	// 今向いている矢印情報が手に入る
	Math::Vector3 nowDir = GetMatrix().Backward();
	//	Math::Vector3 nowDir = GetMatrix().Forward();

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

void Player::UpdateCollision()
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
			SceneManager::Instance().SetNextScene(SceneManager::SceneType::Lose);
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
			maxOverLap = ret.m_overlapDistance;
			hit = true;
			hitDir = ret.m_hitDir;// 押し返す方向
		}
	}
	if (hit)
	{
		Math::Vector3 newPos =
			GetPos() + (hitDir * maxOverLap);
		SetPos(newPos);
	}
}
