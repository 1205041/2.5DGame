#include "Enemy.h"
#include "../Player/Player.h"

// 更新関数
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

	if (!m_isExpired)
	{
		std::shared_ptr<Player> spPlayer = m_wpPlayer.lock();
		if (spPlayer)
		{
			float targetDeg = GetAngleDeg(m_nowPos, spPlayer->GetPos());

			float diffDeg = targetDeg - m_deg;

			//0～359内に補正する
			if (diffDeg < 0) { diffDeg += 360; }

			//旋回方向を決める
			if (diffDeg <= 179)
			{
				//角度差が179以下は左旋回(反時計回り)
				m_deg += m_turnDeg;

				//0～359内に補正する(360以上になっていないか？)
				if (m_deg >= 360) { m_deg -= 360; }
			}
			else
			{
				//角度差が180以上は右旋回(時計回り)
				m_deg -= m_turnDeg;

				//0～359内に補正する(0未満になっていないか？)
				if (m_deg < 0) { m_deg += 360; }
			}

			//移動量を再度求める
			m_moveVec.x = cos(DirectX::XMConvertToRadians(m_deg)) * m_moveSpd;
			m_moveVec.z = sin(DirectX::XMConvertToRadians(m_deg)) * m_moveSpd;
		}
	}
	
	m_moveVec.Normalize();
	m_moveVec *= m_moveSpd;

	m_nowPos += m_moveVec;

	m_spPoly->SetUVRect(Walk[(int)m_anime]);
	m_anime += 0.05f;
	if (m_anime >= 4) { m_anime = 0; }
}

// 更新後更新関数
void Enemy::PostUpdate()
{
	// キャラの座標行列
	m_mWorld = Math::Matrix::CreateTranslation(m_nowPos);
	
	UpdateCollision();
}

// 影描画関数
void Enemy::GenerateDepthMapFromLight()
{
	if (!m_spPoly) { return; }
	KdShaderManager::Instance().m_HD2DShader.DrawPolygon(*m_spPoly, m_mWorld);
}

// 板ポリ描画関数
void Enemy::DrawLit()
{
	if (!m_spPoly) { return; }
	KdShaderManager::Instance().m_HD2DShader.DrawPolygon(*m_spPoly, m_mWorld);
}

// 初期化関数
void Enemy::Init()
{
	if (!m_spPoly)
	{
		m_spPoly = std::make_shared<KdSquarePolygon>();
		m_spPoly->SetMaterial(KdAssets::Instance().m_textures.GetData("Asset/Textures/Enemy/enemy.png"));

		// アニメーション
		m_spPoly->SetPivot(KdSquarePolygon::PivotType::Center_Bottom);
		m_spPoly->SetSplit(4, 4);
	}
	
	m_moveSpd = 0.03f;

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("EnemyCollider", GetPos(), 0.25f, KdCollider::TypeBump);
}
