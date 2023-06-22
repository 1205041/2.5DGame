#include "WinText.h"

void WinText::DrawSprite()
{
	KdShaderManager::Instance().m_HD2DShader.DrawPolygon(m_poly, m_mWorld);
}

void WinText::Init()
{
	m_poly.SetMaterial(KdAssets::Instance().m_textures.GetData("Asset/Textures/win.png"));
}
