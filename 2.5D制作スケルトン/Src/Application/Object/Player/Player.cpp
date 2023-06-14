#include "Player.h"

void Player::Update()
{
	// キャラのアニメーション
	// 24,25,26→24,25,24,26
	int Run[4] = { 24,25,24,26 };
	m_poly.SetUVRect(Run[(int)m_anime]);
	m_anime += 0.2f;
	if (m_anime >= 4) { m_anime = 0; }

	// 実数を扱うときは f を付ける
	// 付いてなかったら、double型
	if (GetAsyncKeyState(VK_UP) & 0x8000) { m_pos.z += 0.1f; }
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) { m_pos.x += 0.1f; }
	if (GetAsyncKeyState(VK_DOWN) & 0x8000) { m_pos.z -= 0.1f; }
	if (GetAsyncKeyState(VK_LEFT) & 0x8000) { m_pos.x -= 0.1f; }

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
	m_anime = 0;
}
