#include "Enemy.h"

void Enemy::Update()
{
	// デバック用
	m_debugWire.AddDebugSphere
	(m_pos + Math::Vector3(0, 0.5f, 0), 0.2f, { 0,1,0,1 });

	// アニメーション
	int Walk[4] = { 3,4,3,5 };
	m_poly.SetUVRect(Walk[(int)m_anime]);
	m_anime += 0.1f;
	if (m_anime >= 4) { m_anime = 0; }

	Math::Matrix transMat;
	// キャラの座標行列
	transMat = Math::Matrix::CreateTranslation(m_pos);
	m_mat = transMat;
}

void Enemy::Draw()
{
	// 板ポリ(キャラ)
	KdShaderManager::Instance().m_HD2DShader.DrawPolygon(m_poly, m_mat);
}

void Enemy::Init()
{
	// 板ポリ(キャラ)初期化
	m_poly.SetMaterial("Asset/Textures/char.png");
	m_mat = Math::Matrix::Identity;
	m_pos = {};
	// 画像中心点移動
	m_poly.SetPivot(KdSquarePolygon::PivotType::Center_Bottom);
	// 画像分割
	m_poly.SetSplit(6, 6);
	m_anime = 0;
}

void Enemy::DrawDebug()
{
}
