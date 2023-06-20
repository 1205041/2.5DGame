#include "SkySphere.h"

void SkySphere::PostUpdate()
{
	// �g�k�s��
	Math::Matrix scaleMat;
	scaleMat = Math::Matrix::CreateScale(20.0f, 20.0f, 20.0f);

	// ���W�s��
	Math::Matrix transMat;
	transMat = Math::Matrix::CreateTranslation({ 0,-5.0f,0 });

	// �s�񍇐�(�r�q�s)
	m_mWorld = scaleMat * transMat;
}

void SkySphere::DrawLit()
{
	// �|��(�n��)
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_model, m_mWorld);
}

void SkySphere::Init()
{
	m_model = std::make_shared<KdModelWork>();
	m_model->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/SkySphere/Doujyou/Doujyou.gltf"));
}
