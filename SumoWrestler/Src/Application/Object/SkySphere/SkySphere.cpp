#include "SkySphere.h"

void SkySphere::PostUpdate()
{
	// Šgks—ñ
	Math::Matrix scaleMat;
	scaleMat = Math::Matrix::CreateScale(20.0f, 20.0f, 20.0f);

	// À•Ws—ñ
	Math::Matrix transMat;
	transMat = Math::Matrix::CreateTranslation({ 0,-5.0f,0 });

	// s—ñ‡¬(‚r‚q‚s)
	m_mWorld = scaleMat * transMat;
}

void SkySphere::DrawLit()
{
	// ”Âƒ|ƒŠ(’n–Ê)
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_model, m_mWorld);
}

void SkySphere::Init()
{
	m_model = std::make_shared<KdModelWork>();
	m_model->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/SkySphere/Doujyou/Doujyou.gltf"));
}
