#include "TitleText.h"

void TitleText::PostUpdate()
{
	// Šgks—ñ
	scaleMat = Math::Matrix::CreateScale(15.0f, 10.0f, 10.0f);

	// s—ñ‡¬(‚r‚q‚s)
	m_mWorld = scaleMat;
}

void TitleText::DrawUnLit()
{
	if (!m_spPoly) { return; }
	KdShaderManager::Instance().m_HD2DShader.DrawPolygon(*m_spPoly, m_mWorld);
}

void TitleText::Init()
{
	if (!m_spPoly)
	{
		m_spPoly = std::make_shared<KdSquarePolygon>();
		m_spPoly->SetMaterial(KdAssets::Instance().m_textures.GetData("Asset/Textures/Title/Title.png"));
	}
}
