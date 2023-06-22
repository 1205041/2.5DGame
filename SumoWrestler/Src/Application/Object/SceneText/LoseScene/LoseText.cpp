#include "LoseText.h"

void LoseText::DrawUnLit()
{
	KdShaderManager::Instance().m_HD2DShader.DrawPolygon(m_poly, m_mWorld);
}

void LoseText::Init()
{
	m_poly.SetMaterial(KdAssets::Instance().m_textures.GetData("Asset/Textures/win.png"));
}