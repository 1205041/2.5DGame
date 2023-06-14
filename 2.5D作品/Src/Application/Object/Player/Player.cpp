#include "Player.h"

#include "../../Scene/GameScene/GameScene.h"
#include "../Ground/Ground.h"

void Player::Update()
{
	// キャラのアニメーション
	// 24,25,26→24,25,24,26
	int Run[4] = { 24,25,24,26 };
	m_poly.SetUVRect(Run[(int)m_anime]);
	m_anime += 0.2f;
	if (m_anime >= 4) { m_anime = 0; }

	if (GetAsyncKeyState(VK_UP) & 0x8000) { m_pos.z += 0.1f; }
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) { m_pos.x += 0.1f; }
	if (GetAsyncKeyState(VK_DOWN) & 0x8000) { m_pos.z -= 0.1f; }
	if (GetAsyncKeyState(VK_LEFT) & 0x8000) { m_pos.x -= 0.1f; }
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		if (!m_jumpFlg)
		{
			m_gravity = -0.1f;
			m_jumpFlg = true;
		}
	}
	else
	{
		m_jumpFlg = false;
	}
	m_pos.y -= m_gravity;
	m_gravity += 0.005f;

	/* ====================== */
	/* 当たり判定(レイ判定用) */
	/* ====================== */
	KdCollider::RayInfo rayInfo;
	// レイの発射位置を設定
	rayInfo.m_pos = m_pos;//m_posはキャラの足元のはず！
	// レイの発射方向を設定
	rayInfo.m_dir = { 0,-1,0 };
	// 少し高い所から飛ばす
	rayInfo.m_pos.y += 0.1f;
	// 段差の許容範囲
	static float enableStepHigh = 0.2f;
	rayInfo.m_pos.y += enableStepHigh;

	// レイの長さを設定
	rayInfo.m_range = m_gravity + enableStepHigh;
	// 当たり判定をしたいタイプを設定
	rayInfo.m_type = KdCollider::TypeGround;

	/* === デバック用 === */
	m_debugWire.AddDebugLine
	(
		rayInfo.m_pos,
		rayInfo.m_dir,
		rayInfo.m_range
	);

	// レイに当たったオブジェクト情報を格納するリスト
	std::list<KdCollider::CollisionResult> retRayList;

	/* レイと当たり判定をする */
	// Groundのレイ情報、対象の行列、格納するリスト
	m_gameScene->GetGround()->GetCollidr().Intersects
	(
		rayInfo,		// レイの情報,
		m_gameScene->GetGround()->GetMat(), // 対象の行列,
		&retRayList		// 当たった情報を格納するリスト
	);

	// Scaffoldのレイ情報、対象の行列、格納するリスト
//	m_gameScene->GetScaffold()->GetCollidr().Intersects
//	(
//		rayInfo,
//		m_gameScene->GetScaffold()->GetMat(),
//		&retRayList
//	);

	// レイに当たったリストから一番近いオブジェクトを検出
	maxOverLap = 0.0f;
	hit = false;
	Math::Vector3 groundPos = Math::Vector3::Zero;
	for (auto& ret : retRayList)
	{
		// レイを遮断してオーバーした長さが
		// 一番長いものを探す
		if (maxOverLap < ret.m_overlapDistance)
		{
			maxOverLap = ret.m_overlapDistance;
			groundPos = ret.m_hitPos;
			hit = true;
		}
	}
	if (hit)
	{
		// 地面に当たっている
		m_pos = groundPos + Math::Vector3(0, -0.1f, 0);
		m_gravity = 0;
	}

	Math::Matrix transMat;
	// キャラの座標行列
	transMat = Math::Matrix::CreateTranslation(m_pos);
	m_mat = transMat;
}

void Player::Draw()
{
	// 板ポリ(キャラ)
	KdShaderManager::Instance().m_HD2DShader.DrawPolygon(m_poly, m_mat);
}

void Player::Init()
{
	//	板ポリ(キャラ)初期化
	m_poly.SetMaterial("Asset/Textures/char.png");
	m_mat = Math::Matrix::Identity;
	m_pos = {};
	m_poly.SetPivot(KdSquarePolygon::PivotType::Center_Bottom);
	m_poly.SetSplit(6, 6);
	m_anime = 0.0f;
}

void Player::DrawDebug()
{
	m_debugWire.Draw();
}
