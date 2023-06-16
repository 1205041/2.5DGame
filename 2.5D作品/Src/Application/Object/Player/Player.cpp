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
	if(GetAsyncKeyState(VK_DOWN) & 0x8000) { m_pos.z -= 0.1f; }
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
	KdCollider::RayInfo rayInfo;// レイの発射位置を設定
	rayInfo.m_pos = m_pos;		//m_posはキャラの足元のはず！
	rayInfo.m_dir = { 0,-1,0 };	// レイの発射方向を設定
	rayInfo.m_pos.y += 0.1f;	// 少し高い所から飛ばす
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
	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		obj->Intersects
		(
			rayInfo,	// レイの情報
			&retRayList	// 当たった情報を格納するリスト
		);
	}

	// レイに当たったリストから一番近いオブジェクトを検出
	float	maxOverLap  = 0.0f;
	bool	hit			= false;
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

	/* ==================== */
	/* 当たり判定(球判定用) */
	/* ==================== */
	KdCollider::SphereInfo sphereInfo;		// 球判定用の変数
	sphereInfo.m_sphere.Center = m_pos + Math::Vector3(0, 0.5f, 0);		// 球の中心位置を設定
	sphereInfo.m_sphere.Radius = 0.3f;									// 球の半径を設定
	sphereInfo.m_type = KdCollider::TypeGround | KdCollider::TypeBump;	// 当たり判定をしたいタイプを設定

	/* === デバック用(球) === */
	m_debugWire.AddDebugSphere(sphereInfo.m_sphere.Center, sphereInfo.m_sphere.Radius);

	// 球に当たったオブジェクト情報を格納するリスト
	std::list<KdCollider::CollisionResult> retSphereList;

	// 球と当たり判定

	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		obj->Intersects
		(
			sphereInfo,		// 球の情報
			&retSphereList	// 当たった情報を格納するリスト
		);
	}

	// 球に当たったリストから一番近いオブジェクトを検出
	maxOverLap = 0.0f;
	hit = false;
	Math::Vector3 hitDir = {};// 当たっている方向
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
		// Zへの押し返し抑止
		hitDir.z = 0;
		hitDir.Normalize();

		// 押し返し
		m_pos += hitDir * maxOverLap;
	}
}

void Player::PostUpdate()
{
	Math::Matrix transMat;
	// キャラの座標行列
	transMat = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = transMat;
}

void Player::GenerateDepthMapFromLight()
{
	// 板ポリ(影)
	KdShaderManager::Instance().m_HD2DShader.DrawPolygon(m_poly, m_mWorld);
}

void Player::DrawLit()
{
	// 板ポリ(キャラ)
	KdShaderManager::Instance().m_HD2DShader.DrawPolygon(m_poly, m_mWorld);
}

void Player::Init()
{
	// キャラ初期値
	m_poly.SetMaterial("Asset/Textures/char.png");
	m_mWorld = Math::Matrix::Identity;
	m_pos = { -20,0,0 };

	// アニメーション初期化
	m_poly.SetPivot(KdSquarePolygon::PivotType::Center_Bottom);
	m_poly.SetSplit(6, 6);
	m_anime = 0.0f;
}

void Player::DrawDebug()
{
	m_debugWire.Draw();
}
