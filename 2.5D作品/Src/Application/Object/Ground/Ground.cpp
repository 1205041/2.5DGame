#include "Ground.h"

void Ground::Draw()
{
	// 板ポリ(地面)
	KdShaderManager::Instance().m_HD2DShader.DrawPolygon(m_poly, m_mat);

}

void Ground::Init()
{
	//	板ポリ(地面)初期化
	m_poly.SetMaterial("Asset/Textures/grass.png");
	m_mat = Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(90));
	m_poly.SetScale(100);
}

