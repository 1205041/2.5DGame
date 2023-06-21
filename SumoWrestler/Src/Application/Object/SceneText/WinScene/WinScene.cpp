#include "WinScene.h"

void WinScene::DrawUnLit()
{
	KdShaderManager::Instance().m_HD2DShader.DrawPolygon(m_poly, m_mWorld);
}

void WinScene::Init()
{
	m_poly.SetMaterial(KdAssets::Instance().m_textures.GetData("Asset/Textures/win.png"));
}
