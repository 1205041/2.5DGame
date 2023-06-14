#include "Ground.h"

void Ground::Draw()
{
	// �|��(�n��)
	KdShaderManager::Instance().m_HD2DShader.DrawPolygon(m_poly, m_mat);

}

void Ground::Init()
{
	//	�|��(�n��)������
	m_poly.SetMaterial("Asset/Textures/grass.png");
	m_mat = Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(90));
	m_poly.SetScale(100);
}

