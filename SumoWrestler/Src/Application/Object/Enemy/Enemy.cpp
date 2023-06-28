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
	Math::Matrix transMat;
	// キャラの座標行列
	transMat = Math::Matrix::CreateTranslation(m_nowPos);
	m_mWorld = transMat;
	
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
	
	m_moveSpd = 0.05f;
	m_moveVec = Math::Vector3::Zero;

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("EnemyCollider", GetPos(), 0.25f, KdCollider::TypeBump);
}
